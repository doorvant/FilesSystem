#ifndef _NAIVEFS_H_
#define _NAIVEFS_H_

#define FUSE_USE_VERSION 26
#include "stdio.h"
#include "stdlib.h"
#include <unistd.h>
#include "fcntl.h"
#include "string.h"
#include "fuse.h"
#include <stddef.h>
#include "ddriver.h"
#include "errno.h"
#include "types.h"

#define NAIVEFS_MAGIC           0x114514       /* TODO: Define by yourself */
#define NAIVEFS_DEFAULT_PERM    0777   		   /* 全权限打开 */
#define NFS_DBG(fmt, ...) do { printf("SFS_DBG: " fmt, ##__VA_ARGS__); } while(0)
/******************************************************************************
* SECTION: 全局变量
*******************************************************************************/
struct custom_options nfs_options;			 /* 全局选项 */
struct nfs_super super; 

/******************************************************************************
* SECTION: naivefs.c
*******************************************************************************/
void* 			   naivefs_init(struct fuse_conn_info *);
void  			   naivefs_destroy(void *);
int   			   naivefs_mkdir(const char *, mode_t);
int   			   naivefs_getattr(const char *, struct stat *);
int   			   naivefs_readdir(const char *, void *, fuse_fill_dir_t, off_t,
						                struct fuse_file_info *);
int   			   naivefs_mknod(const char *, mode_t, dev_t);
int   			   naivefs_write(const char *, const char *, size_t, off_t,
					                  struct fuse_file_info *);
int   			   naivefs_read(const char *, char *, size_t, off_t,
					                 struct fuse_file_info *);
int   			   naivefs_access(const char *, int);
int   			   naivefs_unlink(const char *);
int   			   naivefs_rmdir(const char *);
int   			   naivefs_rename(const char *, const char *);
int   			   naivefs_utimens(const char *, const struct timespec tv[2]);
int   			   naivefs_truncate(const char *, off_t);
			
int   			   naivefs_open(const char *, struct fuse_file_info *);
int   			   naivefs_opendir(const char *, struct fuse_file_info *);

/******************************************************************************
* SECTION: naivefs_funct.c
*******************************************************************************/
int                naivefs_mount(struct custom_options nfs_options);
int                naivefs_umount();
struct nfs_dentry* nfs_lookup(const char * path, int* is_find, int* is_root);
int                nfs_calc_lvl(const char * path);
char* 			   nfs_get_name(const char* path);
/******************************************************************************
* SECTION: naivefs_driver.c
*******************************************************************************/
int 			   nfs_driver_read(int offset, uint8_t* out_content, int size);
int 			   nfs_driver_write(int offset, uint8_t* in_content, int size);

/******************************************************************************
* SECTION: naivefs_struct.c
*******************************************************************************/
struct nfs_inode*  nfs_alloc_inode(struct nfs_dentry * dentry);
int 			   nfs_alloc_dentry(struct nfs_inode* inode, struct nfs_dentry* dentry);
struct nfs_inode*  nfs_read_inode(struct nfs_dentry * dentry, int ino);
int 			   nfs_sync_inode(struct nfs_inode * inode);
struct nfs_dentry* nfs_get_dentry(struct nfs_inode * inode, int dir);

#endif  /* _naivefs_H_ */