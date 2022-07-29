#include "../include/naivefs.h"

/******************************************************************************
* SECTION: 磁盘操作封装
*******************************************************************************/

/**
 * @brief 驱动读
 * 
 * @param offset 
 * @param out_content 
 * @param size 
 * @return int 
 */
int nfs_driver_read(int offset, uint8_t* out_content, int size) {
    // 偏移向下取整，读取大小向上取整
    int      offset_aligned = NFS_ROUND_DOWN(offset, NFS_IO_SZ());
    int      bias           = offset - offset_aligned;
    int      size_aligned   = NFS_ROUND_UP((size+bias), NFS_IO_SZ());
    uint8_t* temp_content   = (uint8_t*)malloc(size_aligned);
    // 指向即将读取处
    uint8_t* cur            = temp_content;

    // 移动磁盘头,移到偏移处
    ddriver_seek(NFS_DRIVER(), offset_aligned, SEEK_SET);
    // 每次读取一个IO单位
    while(size_aligned != 0) {
      ddriver_read(NFS_DRIVER(), cur, NFS_IO_SZ());
      cur          += NFS_IO_SZ();
      size_aligned -= NFS_IO_SZ();
    }
    // 由于之前向下取整，因此复制时要加上bias
    memcpy(out_content, temp_content + bias, size);
    free(temp_content);
    return NFS_ERROR_NONE;
}

/**
 * @brief 驱动写
 * 
 * @param offset 
 * @param in_content 
 * @param size 
 * @return int 
 */
 int nfs_driver_write(int offset, uint8_t *in_content, int size) {
    // 偏移向下取整，写入大小向上取整
    int      offset_aligned = NFS_ROUND_DOWN(offset, NFS_IO_SZ());
    int      bias           = offset - offset_aligned;
    int      size_aligned   = NFS_ROUND_UP((size+bias), NFS_IO_SZ());
    uint8_t* temp_content   = (uint8_t*)malloc(size_aligned);
    // 指向即将写入处
    uint8_t* cur            = temp_content;  
    nfs_driver_read(offset_aligned, temp_content, size_aligned);
    memcpy(temp_content + bias, in_content, size);

    // 移动磁盘头,移到偏移处
    ddriver_seek(NFS_DRIVER(), offset_aligned, SEEK_SET);
    // 每次写入一个IO单位
    while(size_aligned != 0) {
      ddriver_write(NFS_DRIVER(), cur, NFS_IO_SZ());
      cur          += NFS_IO_SZ();
      size_aligned -= NFS_IO_SZ();
    }

    free(temp_content);
    return NFS_ERROR_NONE;
 }