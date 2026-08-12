#include "SecureElement.hpp"

#include "libtropic.h"
#include "libtropic_common.h"
#include "libtropic_mbedtls_v4.h"
#include "libtropic_port_linux_spi.h"
#include "psa/crypto.h"

#define PING_MSG "ping from hsmctl"
#define PING_MSG_SIZE 17

const uint8_t lt_sh0priv_prod0[] = {
    0x28, 0x3f, 0x5a, 0x0f, 0xfc, 0x41, 0xcf, 0x50, 0x98, 0xa8, 0xe1, 0x7d, 0xb6, 0x37, 0x2c, 0x3c,
    0xaa, 0xd1, 0xee, 0xee, 0xdf, 0x0f, 0x75, 0xbc, 0x3f, 0xbf, 0xcd, 0x9c, 0xab, 0x3d, 0xe9, 0x72};

const uint8_t lt_sh0pub_prod0[] = {0xf9, 0x75, 0xeb, 0x3c, 0x2f, 0xd7, 0x90, 0xc9, 0x6f, 0x29, 0x4f,
                                   0x15, 0x57, 0xa5, 0x03, 0x17, 0x80, 0xc9, 0xaa, 0xfa, 0x14, 0x0d,
                                   0xa2, 0x8f, 0x55, 0xe7, 0x51, 0x57, 0x37, 0xb2, 0x50, 0x2c};

struct SecureElement::Impl
{
    lt_handle_t handle = {0};
    lt_dev_linux_spi_t device = {0};
    lt_ctx_mbedtls_v4_t crypto_ctx = {0};
};

SecureElement::SecureElement() : m_impl(std::make_unique<Impl>())
{
}

SecureElement::~SecureElement() = default;

SystemStatus SecureElement::init()
{
    if (psa_crypto_init() != PSA_SUCCESS)
    {
        return SystemStatus::ERROR_INIT;
    }

    snprintf(m_impl->device.spi_dev, sizeof(m_impl->device.spi_dev), "/dev/spidev0.0");
    snprintf(m_impl->device.gpio_dev, sizeof(m_impl->device.gpio_dev), "/dev/gpiochip0");
    m_impl->device.spi_speed = 5000000;
    m_impl->device.gpio_cs_num = 25;

    m_impl->handle.l2.device = &m_impl->device;
    m_impl->handle.l3.crypto_ctx = &m_impl->crypto_ctx;

    if (lt_init(&m_impl->handle) != LT_OK)
    {
        return SystemStatus::ERROR_INIT;
    }

    if (lt_reboot(&m_impl->handle, TR01_REBOOT) != LT_OK)
    {
        lt_deinit(&m_impl->handle);
        mbedtls_psa_crypto_free();
        return SystemStatus::ERROR_INIT;
    }

    if (lt_verify_chip_and_start_secure_session(&m_impl->handle, lt_sh0priv_prod0, lt_sh0pub_prod0,
                                                TR01_PAIRING_KEY_SLOT_INDEX_0) != LT_OK)
    {
        lt_deinit(&m_impl->handle);
        mbedtls_psa_crypto_free();
        return SystemStatus::ERROR_INIT;
    }

    return SystemStatus::OK;
}

SystemStatus SecureElement::ping()
{
    uint8_t recv_buf[PING_MSG_SIZE];

    if (lt_ping(&m_impl->handle, (const uint8_t*)PING_MSG, recv_buf, PING_MSG_SIZE) != LT_OK)
    {
        lt_session_abort(&m_impl->handle);
        lt_deinit(&m_impl->handle);
        mbedtls_psa_crypto_free();
        return SystemStatus::ERROR_PING;
    }

    return SystemStatus::OK;
}

SystemStatus SecureElement::eraseKey(uint8_t slot)
{
    if (lt_ecc_key_erase(&m_impl->handle, static_cast<lt_ecc_slot_t>(slot)) != LT_OK)
    {
        lt_session_abort(&m_impl->handle);
        lt_deinit(&m_impl->handle);
        mbedtls_psa_crypto_free();
        return SystemStatus::ERROR_ERASE_KEY;
    }

    return SystemStatus::OK;
}

SystemStatus SecureElement::generateKey(uint8_t slot, Curve curve)
{
    lt_ecc_curve_type_t lt_curve = (curve == Curve::P256) ? TR01_CURVE_P256 : TR01_CURVE_ED25519;

    if (lt_ecc_key_generate(&m_impl->handle, static_cast<lt_ecc_slot_t>(slot), lt_curve) != LT_OK)
    {
        lt_session_abort(&m_impl->handle);
        lt_deinit(&m_impl->handle);
        mbedtls_psa_crypto_free();
        return SystemStatus::ERROR_GENERATE_KEY;
    }

    return SystemStatus::OK;
}

SystemStatus SecureElement::readKey(uint8_t slot, std::vector<uint8_t>& pubKey)
{
    uint8_t raw_key[64];
    lt_ecc_curve_type_t curve;
    lt_ecc_key_origin_t origin;

    if (lt_ecc_key_read(&m_impl->handle, static_cast<lt_ecc_slot_t>(slot), raw_key, sizeof(raw_key),
                        &curve, &origin) != LT_OK)
    {
        lt_session_abort(&m_impl->handle);
        lt_deinit(&m_impl->handle);
        mbedtls_psa_crypto_free();

        return SystemStatus::ERROR_READ_KEY;
    }

    int key_len = (curve == TR01_CURVE_ED25519) ? 32 : 64;

    pubKey.assign(raw_key, raw_key + key_len);

    return SystemStatus::OK;
}

SystemStatus SecureElement::deinit()
{
    if (lt_session_abort(&m_impl->handle) != LT_OK)
    {
        lt_deinit(&m_impl->handle);
        mbedtls_psa_crypto_free();
        return SystemStatus::ERROR_DEINIT;
    }

    if (lt_deinit(&m_impl->handle) != LT_OK)
    {
        mbedtls_psa_crypto_free();
        return SystemStatus::ERROR_DEINIT;
    }

    mbedtls_psa_crypto_free();

    return SystemStatus::OK;
}