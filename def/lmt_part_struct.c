#include <zephyr/fs/fs.h>
#include <zephyr/kvss/nvs.h>
#include <zephyr/fs/littlefs.h>

// This need to be linked on app build time because overlays scramble the device ordinals.
// Partition 'nvs_storage' NVS FS definition
#define NVS_PARTITION nvs_storage
#define NVS_PARTITION_DEVICE PARTITION_DEVICE(NVS_PARTITION)
#define NVS_PARTITION_OFFSET PARTITION_OFFSET(NVS_PARTITION)
#define NVS_PARTITION_SECTOR_SIZE 4096

struct nvs_fs fs_nvs = {
    .flash_device = NVS_PARTITION_DEVICE,
    .offset = NVS_PARTITION_OFFSET,
    .sector_size = NVS_PARTITION_SECTOR_SIZE,
    .sector_count = PARTITION_SIZE(NVS_PARTITION) / NVS_PARTITION_SECTOR_SIZE,
};

// Partition 'littlefs_storage' LittleFS config
FS_LITTLEFS_DECLARE_DEFAULT_CONFIG(fs_lfs_config);

struct fs_mount_t fs_lfs_mount = {
    .type = FS_LITTLEFS,
    .fs_data = &fs_lfs_config,
    .storage_dev = (void *)PARTITION_ID(lfs1_partition),
    .mnt_point = "/lfs",
};

