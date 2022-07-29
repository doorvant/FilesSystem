#include "../include/naivefs.h"

/******************************************************************************
* SECTION: 数据结构操作
*******************************************************************************/

/**
 * @brief 分配一个inode，占用位图
 * 
 * @param dentry 该dentry指向分配的inode
 * @return nfs_inode
 */
struct nfs_inode* nfs_alloc_inode(struct nfs_dentry * dentry) {
    struct nfs_inode* inode;
    int byte_cur = 0;
    int bit_cur  = 0;
    int ino_cur  = 0;               // 记录已遍历的inode数
    int is_find_free_entry = 0;

    for (byte_cur = 0; byte_cur < super.map_inode_blks * NFS_BLK_SZ(); byte_cur++) {
        for (bit_cur = 0; bit_cur < UINT8_BITS; bit_cur++) {
            // inode位图当前位置空闲
            if ((super.map_inode[byte_cur] & (0x1 << bit_cur)) == 0) {
                super.map_inode[byte_cur] |= (0x1 << bit_cur);
                is_find_free_entry = 1;
                break;
            }
            ino_cur++;
        }
        if(is_find_free_entry) {
            break;
        }
    }
    // 没找到或已经超过最大值
    if (!is_find_free_entry || ino_cur >= super.max_ino) {
        return -NFS_ERROR_NOSPACE;   // error no space
    }

    inode = (struct nfs_inode*)malloc(sizeof(struct nfs_inode));
    inode->ino  = ino_cur;
    inode->size = 0;

    dentry->inode = inode;
    dentry->ino   = inode->ino;

    inode->dentry = dentry;

    inode->dir_cnt = 0;
    inode->dentrys = NULL;
    // 所有指针初始化为-1
    memset(inode->blocks, -1, sizeof(int)*NFS_BLK_PER_FILE);
    // 若不为目录则为文件申请数据空间
    if (!NFS_IS_DIR(inode)) {
        inode->data = (uint8_t*)malloc(NFS_BLK_SZ() * NFS_BLK_PER_FILE);
    }

    return inode;
}

/**
 * @brief 将内存inode及其下方结构全部刷回磁盘
 * 
 * @param inode 
 * @return int 
 */
int nfs_sync_inode(struct nfs_inode * inode) {
    struct nfs_inode_d  inode_d;
    struct nfs_dentry*  dentry_cur;
    struct nfs_dentry_d dentry_d;
    
    int ino         = inode->ino;
    inode_d.ino     = ino;
    inode_d.size    = inode->size;
    inode_d.ftype   = inode->dentry->ftype;
    inode_d.dir_cnt = inode->dir_cnt;
    int offset, i;
    for (i = 0; i < NFS_BLK_PER_FILE; i++) {
        inode_d.blocks[i] = inode->blocks[i];
    }
    // 将inode写入磁盘
    if (nfs_driver_write(NFS_INO_OFS(ino), (uint8_t*)&inode_d,
                         sizeof(struct nfs_inode_d)) != NFS_ERROR_NONE) {
        NFS_DBG("[%s] io error\n", __func__);
        return -NFS_ERROR_IO;
    }
    // 处理目录
    if (NFS_IS_DIR(inode)) {
        // dentry_cur = inode->dentrys;
        // offset     = NFS_DATA_OFS(ino);
        // while(dentry_cur != NULL) {
        //     memcpy(dentry_d.name, dentry_cur->name, MAX_NAME_LEN);
        //     dentry_d.ftype = dentry_cur->ftype;
        //     dentry_d.ino   = dentry_cur->ino;
        //     // 目录项内容写入数据块
        //     if (nfs_driver_write(offset, (uint8_t*)&dentry_d,
        //                          sizeof(struct nfs_dentry_d)) != NFS_ERROR_NONE) {
        //         NFS_DBG("[%s] io error\n", __func__);                     
        //         return -NFS_ERROR_IO;
        //     }
        //     // 递归同步该目录项指向的内存和磁盘inode
        //     if (dentry_cur->inode != NULL) {
        //         nfs_sync_inode(dentry_cur->inode);
        //     }
        //     // 遍历下一个目录项，修改偏移量
        //     dentry_cur = dentry_cur->brother;
        //     offset    += sizeof(struct nfs_dentry_d);
        // }
        int blk_cur = 0;
        int dentry_num = 0;
        if (inode->blocks[blk_cur] == -1) {
            return NFS_ERROR_NONE;
        }
        dentry_cur = inode->dentrys;
        offset     = NFS_DATA_OFS(inode->blocks[blk_cur]);
        while(dentry_cur != NULL) {
            // 当前块放不下目录项，存进下一个块
            if (dentry_num >= super.max_dentry) {
                blk_cur++;
                dentry_num = 0;
                offset = NFS_DATA_OFS(inode->blocks[blk_cur]);
            }
            memcpy(dentry_d.name, dentry_cur->name, MAX_NAME_LEN);
            dentry_d.ftype = dentry_cur->ftype;
            dentry_d.ino   = dentry_cur->ino;
            // 目录项内容写入数据块
            if (nfs_driver_write(offset, (uint8_t*)&dentry_d,
                                 sizeof(struct nfs_dentry_d)) != NFS_ERROR_NONE) {
                NFS_DBG("[%s] io error\n", __func__);                     
                return -NFS_ERROR_IO;
            }
            // 递归同步该目录项指向的内存和磁盘inode
            if (dentry_cur->inode != NULL) {
                nfs_sync_inode(dentry_cur->inode);
            }
            // 遍历下一个目录项，修改偏移量
            dentry_cur = dentry_cur->brother;
            offset    += sizeof(struct nfs_dentry_d);
            dentry_num++;
        }
    } else {  
        // 处理文件，将整个文件写入磁盘
        // if (nfs_driver_write(NFS_DATA_OFS(ino), inode->data,
        //                      NFS_BLK_SZ() * NFS_BLK_PER_FILE) != NFS_ERROR_NONE) {
        //     NFS_DBG("[%s] io error\n", __func__);
        //     return -NFS_ERROR_IO;
        // }
        for (i = 0; i < inode->size; i++) {
            if (nfs_driver_write(NFS_DATA_OFS(inode->blocks[i]), 
                inode->data + i * NFS_BLK_SZ(), NFS_BLK_SZ()) != NFS_ERROR_NONE) {
                NFS_DBG("[%s] io error\n", __func__);
                return -NFS_ERROR_IO;
            }
        }
    }
    return NFS_ERROR_NONE;
}

/**
 * @brief 
 * 
 * @param dentry dentry指向ino，读取该inode
 * @param ino inode唯一编号
 * @return struct nfs_inode* 
 */
struct nfs_inode*  nfs_read_inode(struct nfs_dentry * dentry, int ino) {
    struct nfs_inode* inode = (struct nfs_inode*)malloc(sizeof(struct nfs_inode));
    struct nfs_inode_d  inode_d;
    struct nfs_dentry*  sub_dentry;
    struct nfs_dentry_d dentry_d;
    int    dir_cnt = 0, i;
    if (nfs_driver_read(NFS_INO_OFS(ino), (uint8_t*)&inode_d,
                        sizeof(struct nfs_inode_d)) != NFS_ERROR_NONE) {
        NFS_DBG("[%s] io error\n", __func__);
        return NULL;
    }
    inode->dir_cnt = 0;
    inode->ino     = inode_d.ino;
    inode->size    = inode_d.size;
    inode->dentry  = dentry;
    inode->dentrys = NULL;
    for (i = 0; i < NFS_BLK_PER_FILE; i++) {
        inode->blocks[i] = inode_d.blocks[i];
    }
    if (NFS_IS_DIR(inode)) {
        dir_cnt = inode_d.dir_cnt;
        for (i = 0; i < dir_cnt; i++) {
            // 遍历每一个目录项
            if (nfs_driver_read(NFS_DATA_OFS(ino) + i * sizeof(struct nfs_dentry_d),
               (uint8_t*)&dentry_d, sizeof(struct nfs_dentry_d)) != NFS_ERROR_NONE) {
                NFS_DBG("[%s] io error\n", __func__);
                return NULL;
            }
            sub_dentry = new_dentry(dentry_d.name, dentry_d.ftype);
            sub_dentry->parent = inode->dentry;
            sub_dentry->ino    = dentry_d.ino;
            nfs_alloc_dentry(inode, sub_dentry);
        }
    } else {
        // 读取文件
        inode->data = (uint8_t*)malloc(NFS_BLK_PER_FILE * NFS_BLK_SZ());
        // if (nfs_driver_read(NFS_DATA_OFS(ino), (uint8_t*)inode->data,
        //                     NFS_BLK_PER_FILE * NFS_BLK_SZ()) != NFS_ERROR_NONE) {
        //     NFS_DBG("[%s] io error\n", __func__);
        //     return NULL;
        // }
        for (i = 0; i < inode->size; i++) {
            if (nfs_driver_read(NFS_DATA_OFS(inode->blocks[i]), 
                inode->data + i * NFS_BLK_SZ(), NFS_BLK_SZ()) != NFS_ERROR_NONE) {
                NFS_DBG("[%s] io error\n", __func__);
                return -NFS_ERROR_IO;
            }
        }
    }
    return inode;
}

/**
 * @brief 为一个inode分配dentry，采用头插法
 * 
 * @param inode 
 * @param dentry 
 * @return int 
 */
int nfs_alloc_dentry(struct nfs_inode* inode, struct nfs_dentry* dentry) {
    if (inode->dentrys == NULL) {
        inode->dentrys = dentry;
    }
    else {
        dentry->brother = inode->dentrys;
        inode->dentrys = dentry;
    }
    // 数据块已满
    if (inode->dir_cnt % super.max_dentry == 0) {
        int blk_num = inode->dir_cnt / super.max_dentry;
        if (blk_num == 6) {
            return NFS_ERROR_NOSPACE;
        }
        int byte_cur, bit_cur;
        int blk_cur = 0;
        int is_find = 0;
        for (byte_cur = 0; byte_cur < super.map_data_blks * NFS_BLK_SZ(); byte_cur++) {
            for (bit_cur = 0; bit_cur < UINT8_BITS; bit_cur++) {
                // inode位图当前位置空闲
                if ((super.map_data[byte_cur] & (0x1 << bit_cur)) == 0) {
                    super.map_data[byte_cur] |= (0x1 << bit_cur);
                    is_find = 1;
                    break;
                }
                blk_cur++;
            }
            if(is_find) {
                break;
            }
        }
        if (!is_find || blk_cur >= super.max_data) {
            return -NFS_ERROR_NOSPACE;   // error no space
        }
        inode->blocks[blk_num] = blk_cur;
    }
    inode->dir_cnt++;
    return inode->dir_cnt;
}

/**
 * @brief 获取第dir个目录项
 * 
 * @param inode 
 * @param dir [0...]
 * @return struct nfs_dentry* 
 */
struct nfs_dentry* nfs_get_dentry(struct nfs_inode * inode, int dir) {
    struct nfs_dentry* dentry_cur = inode->dentrys;
    int    cnt = 0;
    while (dentry_cur)
    {
        if (dir == cnt) {
            return dentry_cur;
        }
        cnt++;
        dentry_cur = dentry_cur->brother;
    }
    return NULL;
}