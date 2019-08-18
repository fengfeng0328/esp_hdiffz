#include "esp_hdiffz.h"

#include "unity.h"

#include "sodium.h"
#include "esp_ota_ops.h"

static unsigned char hello_world_diff[] = {
  0x48, 0x44, 0x49, 0x46, 0x46, 0x31, 0x33, 0x26, 0x7a, 0x6c, 0x69, 0x62,
  0x00, 0x89, 0x8d, 0x60, 0x89, 0x8d, 0x50, 0x0c, 0x35, 0x00, 0x87, 0x02,
  0x83, 0x26, 0x84, 0x7d, 0x81, 0x6d, 0x49, 0x00, 0x00, 0x00, 0xd3, 0x7b,
  0x00, 0x0c, 0x93, 0x29, 0x04, 0x00, 0x81, 0xb0, 0x14, 0x34, 0x00, 0x2f,
  0x83, 0x01, 0x1c, 0xc0, 0x70, 0x08, 0x28, 0x40, 0x48, 0x00, 0x81, 0x9f,
  0x27, 0x4d, 0x1b, 0x03, 0x85, 0x59, 0xd2, 0x74, 0x00, 0x8d, 0x18, 0x45,
  0x5c, 0x00, 0xb5, 0x35, 0x08, 0x00, 0x84, 0xdb, 0x38, 0x00, 0x08, 0xb1,
  0x67, 0x0c, 0x48, 0xc7, 0x7d, 0x52, 0xb9, 0x4e, 0xc3, 0x40, 0x10, 0x55,
  0xb0, 0x59, 0x81, 0x15, 0x9b, 0x24, 0xe4, 0x22, 0x17, 0x63, 0xe7, 0x40,
  0x09, 0x48, 0x08, 0xc4, 0x21, 0x40, 0xa4, 0xa0, 0x80, 0x8e, 0x82, 0x2f,
  0x80, 0x0a, 0x89, 0x8a, 0x2a, 0x05, 0x05, 0x9a, 0xbf, 0xa1, 0xe7, 0x0f,
  0x5e, 0x88, 0x25, 0xfe, 0x82, 0x9a, 0x2f, 0xa0, 0xa0, 0x61, 0x76, 0x6d,
  0x56, 0x26, 0x91, 0xd0, 0xdb, 0x9d, 0x63, 0x77, 0x76, 0x66, 0x76, 0x66,
  0x9a, 0x4c, 0xa3, 0x98, 0xfa, 0x6f, 0xb4, 0x1b, 0xe7, 0x3e, 0xe8, 0xeb,
  0x9e, 0x4b, 0xff, 0x80, 0x3e, 0x9f, 0x79, 0x69, 0x01, 0xf4, 0xcc, 0x14,
  0x21, 0x87, 0x65, 0xd9, 0x09, 0xf5, 0x53, 0xb9, 0x3b, 0x64, 0x65, 0x70,
  0x70, 0x64, 0xf4, 0x04, 0xc3, 0x3d, 0xee, 0x3f, 0xc8, 0x3b, 0xc5, 0x74,
  0x99, 0xba, 0x50, 0xd6, 0x99, 0x97, 0x6e, 0x71, 0x7b, 0x64, 0x58, 0xc9,
  0xd0, 0x70, 0x6c, 0x15, 0x95, 0xb1, 0xef, 0x3e, 0x19, 0xd6, 0x90, 0x1d,
  0x05, 0x4c, 0xd7, 0xc2, 0x7b, 0x9b, 0xb1, 0x33, 0xd5, 0x70, 0xa7, 0x6e,
  0x4c, 0xdf, 0x7b, 0x70, 0x66, 0x2e, 0x04, 0x53, 0xf7, 0xbd, 0x85, 0xf3,
  0x3a, 0x8a, 0x33, 0xcf, 0xe4, 0xb7, 0x22, 0x3b, 0x00, 0x85, 0x50, 0xa8,
  0xc2, 0x9d, 0x39, 0xc2, 0xd7, 0xe4, 0xc2, 0x03, 0x1d, 0xcb, 0x8d, 0x42,
  0x39, 0x43, 0x8b, 0x86, 0xd2, 0x96, 0x30, 0x1a, 0xa0, 0x02, 0x3a, 0xc8,
  0x1c, 0x37, 0xc5, 0x55, 0xb8, 0x2a, 0x62, 0x8b, 0xdb, 0x5c, 0x44, 0x85,
  0x8b, 0xec, 0x72, 0x1b, 0x3e, 0xd3, 0x36, 0x68, 0x5f, 0xf8, 0x1a, 0xf2,
  0x28, 0xa5, 0xc8, 0x83, 0x0e, 0xb9, 0x82, 0x02, 0x1c, 0xd4, 0x24, 0x5a,
  0x5d, 0x24, 0x5f, 0xe4, 0xb6, 0x89, 0xed, 0x88, 0x4e, 0x11, 0xeb, 0x35,
  0xd2, 0xd4, 0x95, 0xb3, 0x15, 0x94, 0x8d, 0xc8, 0x81, 0x88, 0x3e, 0x3a,
  0x89, 0xc4, 0x65, 0x50, 0x0f, 0xd4, 0x65, 0x3a, 0xe1, 0x02, 0x07, 0x62,
  0x59, 0x00, 0xf5, 0x41, 0x43, 0x90, 0x7c, 0x18, 0xb4, 0x83, 0x75, 0x84,
  0xb7, 0x52, 0x1e, 0x39, 0xe2, 0xd0, 0xe7, 0x1a, 0xf7, 0x37, 0xb8, 0xc9,
  0x61, 0x4e, 0x7f, 0xe3, 0x8a, 0xe9, 0xe5, 0x4c, 0x57, 0x6a, 0xc2, 0x55,
  0x81, 0x6d, 0xe3, 0x45, 0xc2, 0x6e, 0x74, 0x95, 0x91, 0x9b, 0x6f, 0xb3,
  0x6e, 0x9d, 0xf0, 0x4e, 0xb6, 0x37, 0xa6, 0x11, 0xa9, 0xa9, 0xfa, 0xfb,
  0x28, 0x35, 0x9f, 0x87, 0x34, 0x69, 0x90, 0xbc, 0xaf, 0x2e, 0x78, 0xa8,
  0xa6, 0x9d, 0x5c, 0xca, 0x0c, 0x85, 0xb2, 0x6d, 0x4f, 0x6e, 0x68, 0x6c,
  0xa7, 0x44, 0x9b, 0xd3, 0x64, 0x61, 0x80, 0x7e, 0xb5, 0xc0, 0xc6, 0xa7,
  0x53, 0x93, 0x50, 0xc5, 0x4e, 0x98, 0xf7, 0xf7, 0xc7, 0xd9, 0x48, 0x6a,
  0x21, 0x63, 0x6f, 0x6e, 0xe4, 0xd2, 0xdf, 0x59, 0x5d, 0x99, 0x44, 0x3a,
  0x99, 0x42, 0x66, 0x51, 0xb2, 0xe3, 0xdc, 0x10, 0x44, 0x77, 0x08, 0x1f,
  0x63, 0xd7, 0xa0, 0xf7, 0x36, 0xd2, 0x29, 0xbc, 0x36, 0x7e, 0x00, 0x6c,
  0xf0, 0x17, 0x6d, 0x0c, 0x48, 0xc7, 0xe3, 0xd0, 0x4d, 0xc9, 0x2c, 0x2a,
  0xa9, 0x64, 0x62, 0xf8, 0xf3, 0x1f, 0xc2, 0xb2, 0x7c, 0xb7, 0xb0, 0x31,
  0x74, 0xc9, 0xdd, 0x94, 0xb0, 0x8c, 0x5f, 0x73, 0xb3, 0x17, 0xed, 0xfe,
  0x94, 0xb5, 0x43, 0xfd, 0xd1, 0xc4, 0x94, 0x4d, 0x2f, 0x83, 0x58, 0x8e,
  0x4d, 0x51, 0xcd, 0xb4, 0x7e, 0xc0, 0x81, 0x0a, 0x78, 0xe0, 0x80, 0xe3,
  0x0e, 0x33, 0x10, 0x32, 0x81, 0x49, 0x24, 0x08, 0x52, 0x04, 0xe7, 0x01,
  0x65, 0x7f, 0x80, 0x94, 0x12, 0x07, 0x1a, 0xa6, 0xff, 0x6f, 0x98, 0xf6,
  0x1f, 0x85, 0x84, 0x32, 0x40, 0xc4, 0x74, 0x10, 0x85, 0x2a, 0x33, 0x93,
  0x71, 0x26, 0x23, 0xc3, 0x32, 0x26, 0x10, 0x05, 0x21, 0x21, 0xac, 0x86,
  0x2c, 0x90, 0x30, 0xe3, 0x32, 0xc6, 0x99, 0x48, 0x70, 0x3d, 0x58, 0x6a,
  0x16, 0x8c, 0x04, 0x62, 0xa0, 0x16, 0x26, 0x30, 0x06, 0xf1, 0x80, 0x10,
  0x46, 0xce, 0x64, 0xf2, 0xfc, 0x0f, 0x84, 0x31, 0x20, 0x1c, 0x04, 0x24,
  0x3c, 0xff, 0x47, 0xff, 0xf7, 0x8c, 0xfe, 0xef, 0x09, 0x87, 0x0e, 0x97,
  0x80, 0x62, 0x31, 0xff, 0x83, 0x80, 0xe2, 0x31, 0x50, 0xa1, 0xe8, 0xff,
  0xd1, 0x08, 0x1a, 0x08, 0xa0, 0xca, 0x3d, 0xa3, 0xa1, 0x7a, 0xa2, 0xc1,
  0xe6, 0x00, 0xe5, 0xc1, 0x72, 0xd1, 0x28, 0xba, 0x84, 0x84, 0x18, 0xbf,
  0x80, 0xc1, 0x7f, 0x20, 0x44, 0x0d, 0x62, 0x0e, 0x46, 0x38, 0x8f, 0x11,
  0x08, 0x11, 0x80, 0x11, 0x89, 0x44, 0x93, 0x82, 0x47, 0x14, 0x23, 0x07,
  0xd1, 0x80, 0x11, 0x8d, 0x26, 0x52, 0x39, 0xd6, 0x48, 0x84, 0x44, 0x16,
  0x0c, 0x21, 0xe2, 0xf0, 0xff, 0x0f, 0x0e, 0x00, 0xe9, 0x19, 0xc8, 0xf7,
  0x20, 0x61, 0x66, 0x74, 0x65, 0x72, 0x20, 0x70, 0x61, 0x74, 0x63, 0x68,
  0x70, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xff, 0xff, 0x0f, 0x21, 0xf7, 0xeb,
  0x48, 0x0b, 0x97, 0x14, 0x0a, 0x1b, 0x88, 0xa6, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xc6, 0xb8, 0x6c, 0x28, 0x12, 0x55, 0x78, 0x0f,
  0xd2, 0x53, 0x69, 0x1c, 0xf8, 0x89, 0x79, 0xed, 0x7a, 0x73, 0xcf, 0x43,
  0xc7, 0x80, 0x18, 0xa5, 0x7d, 0x8e, 0x1e, 0x60, 0x14, 0x97, 0xed, 0xee,
  0x28
};

static void print_partition_hash( const char *msg, const esp_partition_t *part ){
    uint8_t sha256[32];
    TEST_ESP_OK(esp_partition_get_sha256(part, sha256));
    char hex[65];
    sodium_bin2hex( hex, sizeof(hex), sha256, 32 );
    printf("%s%s", msg, hex);
}

/**
 * Proxy for testing OTA update.
 *
 * Make sure all the data is flashed via flash-unit-test.sh
 *
 * The firmwares are as follows:
 *     ota_0 - old firmware.
 *     ota_1 - partition to flash the patched firmware.
 *     ota_2 - what the patched firmware should be.
 */
TEST_CASE("ota", "[hdiffz]")
{
    {
        const esp_partition_t *src = esp_ota_get_running_partition();
        TEST_ASSERT_NOT_NULL(src);
        printf( "Running partition type %d subtype %d (offset 0x%08x)",
                src->type, src->subtype, src->address);
    }

    const esp_partition_t *ota_0, *ota_1, *ota_2;
    uint8_t ota_0_sha256[32], ota_1_sha256[32], ota_2_sha256[32];

    ota_0 = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_0, NULL);
    TEST_ASSERT_NOT_NULL(ota_0);

    ota_1 = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_1, NULL);
    TEST_ASSERT_NOT_NULL(ota_1);
    TEST_ESP_OK(esp_partition_erase_range(ota_1, 0, ota_1->size)); // Ensure there's nothing left over in target partition

    ota_2 = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_2, NULL);
    TEST_ASSERT_NOT_NULL(ota_2);

    TEST_ESP_OK(esp_partition_get_sha256(ota_0, ota_0_sha256));
    TEST_ESP_OK(esp_partition_get_sha256(ota_1, ota_1_sha256));
    TEST_ESP_OK(esp_partition_get_sha256(ota_2, ota_2_sha256));

    print_partition_hash("ota_0: ", ota_0);
    print_partition_hash("ota_1: ", ota_1);
    print_partition_hash("ota_2: ", ota_2);


#if 0
    esp_err_t err;
    err = esp_hdiffz_ota_begin_adv(const esp_partition_t *src, const esp_partition_t *dst, size_t image_size, esp_hdiffz_ota_handle_t **out_handle);
#endif
}
