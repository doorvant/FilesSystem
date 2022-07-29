#ifndef _TYPES_H_
#define _TYPES_H_

/******************************************************************************
* SECTION: Macro
*******************************************************************************/

#define MAX_NAME_LEN            128     
#define MAX_INODE_PTR           6         // 一个inode最多指向的数据块数

#define NFS_SUPER_OFS           0         // super block偏移
#define NFS_ROOT_INO            0         // root ino号

#define NFS_BLK_PER_FILE        6
#define NFS_INO_PER_FILE        1

#define NFS_ERROR_NONE          0  
#define NFS_ERROR_IO            EIO
#define NFS_ERROR_NOSPACE       ENOSPC
#define NFS_ERROR_NOTFOUND      ENOENT
#define NFS_ERROR_EXISTS        EEXIST
#define NFS_ERROR_UNSUPPORTED   ENXIO

#define UINT8_BITS              8

/******************************************************************************
* SECTION: Macro Function
*******************************************************************************/
#define NFS_IO_SZ()                     (super.size_io)
#define NFS_BLK_SZ()                    (super.size_io * 2)
#define NFS_DISK_SZ()                   (super.size_disk)
#define NFS_BLK_NUM()                   (super.size_disk / (super.size_io * 2))
#define NFS_DRIVER()                    (super.driver_fd)

#define NFS_ROUND_DOWN(value, round)    (value % round == 0 ? value : (value / round) * round) 
#define NFS_ROUND_UP(value, round)      (value % round == 0 ? value : (value / round + 1) * round)

#define NFS_IS_DIR(pinode)              (pinode->dentry->ftype == NFS_DIR)

#define NFS_INO_OFS(ino)                (super.inode_offset + ino * NFS_BLK_SZ())
#define NFS_DATA_OFS(ino)               (super.data_offset + ino * NFS_BLK_SZ() * NFS_BLK_PER_FILE)    

/******************************************************************************
* SECTION: FS Specific Structure - In memory structure
*******************************************************************************/

struct nfs_inode;
struct nfs_dentry;

typedef enum file_type {
    NFS_FILE,           // 普通文件
    NFS_DIR             // 目录文件
} FILE_TYPE;

struct custom_options {
	char*                  device;
};

struct nfs_super {
    int                driver_fd;         // 控制的设备号
    int                size_disk;         // 磁盘大小
    int                size_io;           // 驱动读写大小
    int                size_usage;        // 磁盘已用大小
    int                max_ino;           // 最多支持的文件数
    int                max_data;          // 总数据块数
    int                max_dentry;        // 每个数据块最多支持的目录项数
    uint8_t*           map_inode;         // inode位图指针
    int                map_inode_blks;    // inode位图占用的块数
    int                map_inode_offset;  // inode位图在磁盘上的偏移
    uint8_t*           map_data;          // data位图指针
    int                map_data_blks;     // data位图占用的块数
    int                map_data_offset;   // data位图在磁盘上的偏移
    int                inode_offset;      // inode在磁盘上的偏移
    int                data_offset;       // 数据块在磁盘上的偏移
    int                is_mounted;        // 文件系统是否已被装载
    struct nfs_dentry* root_dentry;       // 根目录
};

struct nfs_inode {
    uint32_t            ino;                     // inode号
    int                 size;                    // 文件大小
    int                 dir_cnt;                 // 目录项数量
    struct nfs_dentry*  dentry;                  // 指向该inode的目录项
    struct nfs_dentry*  dentrys;                 // 该inode指向的第一个目录项
    int                 blocks[MAX_INODE_PTR];   // 磁盘数据块指针
    uint8_t*            data;                    // 文件内的数据        
};

struct nfs_dentry {
    char               name[MAX_NAME_LEN];   // 文件名
    FILE_TYPE          ftype;                // 文件类型
    struct nfs_dentry* parent;               // 上一级目录项
    struct nfs_dentry* brother;              // 同级目录的目录项  
    uint32_t           ino;                  // 对应inode的inode号        
    struct nfs_inode*  inode;                // 指向inode
    int                valid;                // 该目录项是否有效
};

static inline struct nfs_dentry* new_dentry(char* name, FILE_TYPE ftype) {
    struct nfs_dentry* dentry = (struct nfs_dentry*)malloc(sizeof(struct nfs_dentry));
    memset(dentry, 0, sizeof(struct nfs_dentry));
    memcpy(dentry->name, name, strlen(name));
    dentry->ftype = ftype;
    dentry->ino = -1;
    dentry->inode = NULL;
    dentry->parent = NULL;
    dentry->brother = NULL;
}

/******************************************************************************
* SECTION: FS Specific Structure - Disk structure
*******************************************************************************/
struct nfs_super_d
{
    uint32_t magic_num;          // 幻数
    int      size_usage;         // 磁盘已用空间
    int      max_ino;            // 文件系统最多支持的文件数
    int      max_data;           // 总数据块数
    int      map_inode_blks;     // inode位图占用的块数
    int      map_inode_offset;   // inode位图在磁盘上的偏移    
    int      map_data_blks;      // 数据位图占用的块数
    int      map_data_offset;    // 数据位图在磁盘上的偏移   
    int      inode_offset;       // inode在磁盘上的偏移
    int      data_offset;        // 数据块在磁盘上的偏移
};

struct nfs_inode_d
{
    int        ino;                    // inode号
    int        size;                   // 文件大小
    int        dir_cnt;                // 目录项数量
    FILE_TYPE  ftype;                  // 文件类型
    int        blocks[MAX_INODE_PTR];  // 磁盘数据块指针
};  

struct nfs_dentry_d
{
    char               name[MAX_NAME_LEN];          // 文件名
    FILE_TYPE          ftype;                       // 文件类型
    int                ino;                         // 指向的ino号 
};  


#endif /* _TYPES_H_ */