#include "../include/naivefs.h"

/******************************************************************************
* SECTION: 功能函数
*******************************************************************************/

 /**
 * @brief 挂载naivefs, Layout 如下
 * 
 * Layout
 * | Super | Inode Bitmap | Data Bitmap| Inode | Data |
 * 
 * 2*IO_SZ = BLK_SZ 
 * 
 * 每个Inode占用一个Blk
 * @param options 
 * @return int    
 */
int naivefs_mount(struct custom_options nfs_options) {
   int                     ret = NFS_ERROR_NONE;
   int                     driver_fd;
   struct nfs_super_d      nfs_super_d; 
   struct nfs_dentry*      root_dentry; 
   struct nfs_inode*       root_inode;

   int                     super_blks;
   int                     inode_num;
   int                     inode_blks;
   int                     map_data_blks;
   int                     map_inode_blks;

   int                     is_init = 0;

   super.is_mounted = 0;

   // 控制设备
   driver_fd = ddriver_open(nfs_options.device);
   if (driver_fd < 0) {
      return driver_fd;
   }
   super.driver_fd = driver_fd;
   // 获取磁盘容量和IO大小
   ddriver_ioctl(NFS_DRIVER(), IOC_REQ_DEVICE_SIZE,  &super.size_disk);
   ddriver_ioctl(NFS_DRIVER(), IOC_REQ_DEVICE_IO_SZ, &super.size_io);
   
   // 初始化根目录
   root_dentry = new_dentry("/", NFS_DIR);

   // 读取磁盘中的super block
   if (nfs_driver_read(NFS_SUPER_OFS, (uint8_t*)(&nfs_super_d),
                       sizeof(struct nfs_super_d)) != NFS_ERROR_NONE ) {
      NFS_DBG("[%s] io error\n", __func__);
      return -NFS_ERROR_IO;     // error input/output
   }
   // 估算各部分大小
   if (nfs_super_d.magic_num != NAIVEFS_MAGIC) {
      super_blks = NFS_ROUND_UP(sizeof(struct nfs_super_d), NFS_BLK_SZ()) 
                   / NFS_BLK_SZ();
      // 假设每个文件大小为6k, 估计inode数
      inode_num  = NFS_DISK_SZ() 
                   / ((NFS_BLK_PER_FILE + NFS_INO_PER_FILE) * NFS_BLK_SZ());
      // inode位图所占块数
      map_inode_blks = NFS_ROUND_UP(inode_num, NFS_BLK_SZ()) / NFS_BLK_SZ();
      // inode所占块数,假设一个inode占一块
      inode_blks = inode_num;
      // data位图所占块数
      int remain_blks = NFS_BLK_NUM() - super_blks - inode_blks - map_inode_blks;
      map_data_blks =  NFS_ROUND_UP(remain_blks, NFS_BLK_SZ()) / NFS_BLK_SZ();   
      // 存放数据的块数              
      remain_blks -= map_data_blks; 
      // 布局
      super.max_ino                = inode_num;
      nfs_super_d.max_ino          = inode_num;
      super.max_data               = remain_blks;
      nfs_super_d.max_data         = remain_blks;
      nfs_super_d.map_inode_offset = NFS_SUPER_OFS + super_blks * NFS_BLK_SZ();
      nfs_super_d.map_data_offset  = nfs_super_d.map_inode_offset 
                                     + map_inode_blks * NFS_BLK_SZ();
      nfs_super_d.inode_offset     = nfs_super_d.map_data_offset
                                     + map_data_blks * NFS_BLK_SZ();
      nfs_super_d.data_offset      = nfs_super_d.inode_offset
                                     + inode_num * NFS_BLK_SZ();
      nfs_super_d.map_inode_blks   = map_inode_blks;
      nfs_super_d.map_data_blks    = map_data_blks;
      nfs_super_d.size_usage         = 0;

      is_init = 1;
   }
   // 内存结构
   super.size_usage       = nfs_super_d.size_usage;
   super.map_inode        = (uint8_t*)malloc(NFS_BLK_SZ() * nfs_super_d.map_inode_blks);
   super.map_data         = (uint8_t*)malloc(NFS_BLK_SZ() * nfs_super_d.map_data_blks);
   super.max_dentry       = NFS_BLK_SZ() / sizeof(struct nfs_dentry_d);
   super.map_inode_blks   = nfs_super_d.map_inode_blks;
   super.map_data_blks    = nfs_super_d.map_data_blks;
   super.map_inode_offset = nfs_super_d.map_inode_offset;
   super.map_data_offset  = nfs_super_d.map_data_offset;
   super.inode_offset     = nfs_super_d.inode_offset;
   super.data_offset      = nfs_super_d.data_offset;

   // 读取inode位图
   if (nfs_driver_read(nfs_super_d.map_inode_offset, (uint8_t*)(super.map_inode),
                       super.map_inode_blks * NFS_BLK_SZ()) != NFS_ERROR_NONE) {
      NFS_DBG("[%s] io error\n", __func__);
      return -NFS_ERROR_IO;
   }
   // 读取data位图
   if (nfs_driver_read(nfs_super_d.map_data_offset, (uint8_t*)(super.map_data),
                       super.map_data_blks * NFS_BLK_SZ()) != NFS_ERROR_NONE) {
      NFS_DBG("[%s] io error\n", __func__);
      return -NFS_ERROR_IO;
   }
   // 分配根节点并与磁盘同步
   if(is_init) {
      root_inode = nfs_alloc_inode(root_dentry);

      nfs_sync_inode(root_inode);
   }
   // 从磁盘读取根inode
   root_inode         = nfs_read_inode(root_dentry, NFS_ROOT_INO);
   root_dentry->inode = root_inode;
   super.root_dentry  = root_dentry;
   super.is_mounted   = 1;

   return ret;
}

/**
 * @brief 
 * 
 * @return int 
 */
int naivefs_umount() {
   struct nfs_super_d nfs_super_d;

   if (!super.is_mounted) {
      return NFS_ERROR_NONE;
   }

   nfs_sync_inode(super.root_dentry->inode);

   nfs_super_d.magic_num         = NAIVEFS_MAGIC;
   nfs_super_d.map_inode_blks    = super.map_inode_blks;
   nfs_super_d.map_data_blks     = super.map_data_blks;
   nfs_super_d.map_inode_offset  = super.map_inode_offset;
   nfs_super_d.map_data_offset   = super.map_data_offset;
   nfs_super_d.inode_offset      = super.inode_offset;
   nfs_super_d.data_offset       = super.data_offset; 
   nfs_super_d.size_usage        = super.size_usage;

   // 写回超级块
   if (nfs_driver_write(NFS_SUPER_OFS, (uint8_t *)&nfs_super_d, 
                     sizeof(struct nfs_super_d)) != NFS_ERROR_NONE) {
      NFS_DBG("[%s] io error\n", __func__);
      return -NFS_ERROR_IO;
   }

   // 写回inode位图
   if (nfs_driver_write(nfs_super_d.map_inode_offset, (uint8_t *)(super.map_inode), 
                        nfs_super_d.map_inode_blks * NFS_BLK_SZ()) != NFS_ERROR_NONE) {
      NFS_DBG("[%s] io error\n", __func__);
      return -NFS_ERROR_IO;
   }

   // 写回数据位图
   if (nfs_driver_write(nfs_super_d.map_data_offset, (uint8_t *)(super.map_data), 
                        nfs_super_d.map_data_blks * NFS_BLK_SZ()) != NFS_ERROR_NONE) {
      NFS_DBG("[%s] io error\n", __func__);
      return -NFS_ERROR_IO;
   }

   free(super.map_inode);
   free(super.map_data);
   ddriver_close(NFS_DRIVER());

   return NFS_ERROR_NONE;
}

/**
 * @brief 
 * path: /qwe/ad  total_lvl = 2,
 *      1) find /'s inode       lvl = 1
 *      2) find qwe's dentry 
 *      3) find qwe's inode     lvl = 2
 *      4) find ad's dentry
 *
 * path: /qwe     total_lvl = 1,
 *      1) find /'s inode       lvl = 1
 *      2) find qwe's dentry
 * 
 * @param path 
 * @return struct nfs_inode* 
 */
struct nfs_dentry* nfs_lookup(const char * path, int* is_find, int* is_root) {
   struct nfs_dentry* dentry_cur = super.root_dentry;
   struct nfs_dentry* dentry_ret = NULL;
   struct nfs_inode*  inode;
   // 计算路径的层级
   int total_lvl = nfs_calc_lvl(path); 
   int lvl = 0;
   int is_hit;
   char* name = NULL;
   char* path_cpy = (char*)malloc(sizeof(path));
   *is_root = 0;
   // 复制路径
   strcpy(path_cpy, path); 
   // 根目录
   if (total_lvl == 0) {
      *is_find = 1;
      *is_root = 1;
      dentry_ret = super.root_dentry;
   }
   // 获取每个“/”间隔间的名字
   name = strtok(path_cpy, "/");
   while (name) {
      lvl++;
      //
      if (dentry_cur->inode == NULL) { // Cache机制
         dentry_cur->inode = nfs_read_inode(dentry_cur, dentry_cur->ino);
      }

      inode = dentry_cur->inode;

      if (!NFS_IS_DIR(inode) && lvl < total_lvl) {
         NFS_DBG("[%s] not a dir\n", __func__);
         dentry_ret = inode->dentry;
         break;
      }
      if (NFS_IS_DIR(inode)) {
         dentry_cur = inode->dentrys;
         is_hit     = 0;

         while (dentry_cur) {
            // 遍历目录项，找到了就跳出
            if (memcmp(dentry_cur->name, name, strlen(name)) == 0) {
               is_hit = 1;
               break;
            }
            dentry_cur = dentry_cur->brother;
         }

         if (!is_hit) {
            *is_find = 0;
            NFS_DBG("[%s] not found %s\n", __func__, name);
            dentry_ret = inode->dentry;
            break;
         }
         // 找到了，且路径层级吻合
         if (is_hit && lvl == total_lvl) {
            *is_find = 1;
            dentry_ret = dentry_cur;
            break;
         }
      }
      name = strtok(NULL, "/");
   }
   // 读inode
   if (dentry_ret->inode == NULL) {
       dentry_ret->inode = nfs_read_inode(dentry_ret, dentry_ret->ino);
   }

   return dentry_ret;
}

/**
 * @brief 计算路径的层级
 * exm: /av/c/d/f
 * -> lvl = 4
 * @param path 
 * @return int 
 */
int nfs_calc_lvl(const char * path) {
   char* str = path;
   int   lvl = 0;
   if (strcmp(path, "/") == 0) {
      return lvl;      
   }
   while (*str != NULL) {
      if (*str == '/') {
         lvl++;
      }
      str++;
   }
   return lvl;
}

/**
 * @brief 获取文件名
 * 
 * @param path 
 * @return char* 
 */
char* nfs_get_name(const char* path) {
    char ch = '/';
    // 从后向前寻找字符
    char *q = strrchr(path, ch) + 1;
    return q;
}