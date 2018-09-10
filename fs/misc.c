/*************************************************************************//**
 *****************************************************************************
 * @file   misc.c
 * @brief  
 * @author Forrest Y. Yu
 * @date   2008
 *****************************************************************************
 *****************************************************************************/

/* Orange'S FS */

#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "keyboard.h"
#include "proto.h"
#include "hd.h"
#include "fs.h"

int open_dir(struct inode * last,struct inode ** next,char * filename);

/*****************************************************************************
 *                                search_file
 *****************************************************************************/
/**
 * Search the file and return the inode_nr.
 *
 * @param[in] path The full path of the file to search.
 * @return         Ptr to the i-node of the file if successful, otherwise zero.
 * 
 * @see open()
 * @see do_open()
 *****************************************************************************/
PUBLIC int search_file(char * path)
{
	int i, j;

	char filename[MAX_PATH];
	memset(filename, 0, MAX_FILENAME_LEN);
	struct inode * dir_inode;
	if (strip_path(filename, path, &dir_inode) != 0)
		return 0;

	if (filename[0] == 0)	/* path: "/" */
		return dir_inode->i_num;

	/**
	 * Search the dir for the file.
	 */
	int dir_blk0_nr = dir_inode->i_start_sect;
	printl("sector%d\n",dir_blk0_nr );
	int nr_dir_blks = (dir_inode->i_size + SECTOR_SIZE - 1) / SECTOR_SIZE;
	int nr_dir_entries =
	  dir_inode->i_size / DIR_ENTRY_SIZE; /**
					       * including unused slots
					       * (the file has been deleted
					       * but the slot is still there)
					       */
	int m = 0;
	printl("i_size:%d\n",nr_dir_entries);

	if(dir_inode==root_inode)
			printl("From root!Sector should be %d\n",root_inode->i_start_sect);
	struct dir_entry * pde;
	for (i = 0; i < nr_dir_blks; i++) {
		RD_SECT(dir_inode->i_dev, dir_blk0_nr + i);
		pde = (struct dir_entry *)fsbuf;
		printl("search\n");
		for (j = 0; j < SECTOR_SIZE / DIR_ENTRY_SIZE; j++,pde++) {
			printl("%s %d\n",pde->name,pde->inode_nr);
			if (memcmp(filename, pde->name, MAX_FILENAME_LEN) == 0)
				{
					return pde->inode_nr;
				}
			if (++m >= nr_dir_entries)
				break;
		}
		if (m >= nr_dir_entries) /* all entries have been iterated */
			break;
	}

	/* file not found */
	return 0;
}

/*****************************************************************************
 *                                strip_path
 *****************************************************************************/
/**
 * Get the basename from the fullpath.
 *
 * In Orange'S FS v1.0, all files are stored in the root directory.
 * There is no sub-folder thing.
 *
 * This routine should be called at the very beginning of file operations
 * such as open(), read() and write(). It accepts the full path and returns
 * two things: the basename and a ptr of the root dir's i-node.
 *
 * e.g. After stip_path(filename, "/blah", ppinode) finishes, we get:
 *      - filename: "blah"
 *      - *ppinode: root_inode
 *      - ret val:  0 (successful)
 *
 * Currently an acceptable pathname should begin with at most one `/'
 * preceding a filename.
 *
 * Filenames may contain any character except '/' and '\\0'.
 *
 * @param[out] filename The string for the result.
 * @param[in]  pathname The full pathname.
 * @param[out] ppinode  The ptr of the dir's inode will be stored here.
 * 
 * @return Zero if success, otherwise the pathname is not valid.
 *****************************************************************************/
PUBLIC int strip_path(char * filename, const char * pathname,
		      struct inode** ppinode)
{
	printl("root:%d\n",root_inode->i_start_sect);
	const char * s = pathname;
	char * t = filename;
	struct inode *last=0;
	struct inode *next=0;
	if (s == 0)
		return -1;

	if (*s == '/')
	{
		s++;
		last = root_inode;
	}
	else 
	{
		last = currentDir_inode;
	}
	//在此处解析	
	
	while (*s) {		/* check each character */
		if (*s == '/')
		{
			
			*t = 0;
			t=filename;
			printl("filename:%s\n",filename);
			//bug 要求一个函数实现 给出1.父文件夹 2.子文件夹名 返回子文件夹的*inode。
			if(open_dir(last,&next,filename)==-1)
			{
				printl("open fail\n");
				return -1;
			}
			memset(filename, 0, MAX_FILENAME_LEN);
			last=next;
			s++;
		}
			
		*t++ = *s++;
		/* if filename is too long, just truncate it */
		if (t - filename >= MAX_FILENAME_LEN)
			break;
	}
	*t = 0;
printl("filename:%s\n",filename);
if(last->i_num==root_inode->i_num)
{
	*ppinode=get_inode(root_inode->i_dev, root_inode->i_num);
}
else{
    *ppinode=last;
}

	return 0;
}
//bug
int open_dir(struct inode * last,struct inode ** next,char * filename)
{
	printl("want to open%s\n",filename);
	int i,j;
	int inode_nr=0;
	int dir_blk0_nr = (last)->i_start_sect;
	printl("sector%d\n",dir_blk0_nr );
	int nr_dir_blks = ((last)->i_size + SECTOR_SIZE - 1) / SECTOR_SIZE;
	int nr_dir_entries =
	  (last)->i_size / DIR_ENTRY_SIZE; /**
					       * including unused slots
					       * (the file has been deleted
					       * but the slot is still there)
					       */
	int m = 0;
	printl("i_size:%d\n",nr_dir_entries);

	if((last)==root_inode)
			printl("From root!Sector should be %d\n",root_inode->i_start_sect);
	struct dir_entry * pde;
	for (i = 0; i < nr_dir_blks; i++) {
		RD_SECT((last)->i_dev, dir_blk0_nr + i);
		pde = (struct dir_entry *)fsbuf;
		printl("search\n");
		for (j = 0; j < SECTOR_SIZE / DIR_ENTRY_SIZE; j++,pde++) {
			printl("%s %d\n",pde->name,pde->inode_nr);
			if (memcmp(filename, pde->name,strlen(filename)) == 0)
				{
					printl("find%s %d\n",pde->name,pde->inode_nr);
					inode_nr = pde->inode_nr;
					break;
				}
			if (++m >= nr_dir_entries)
				break;
		}
		if (m >= nr_dir_entries) /* all entries have been iterated */
			break;
	}
	
	if(inode_nr==0)
	    return -1;
	
	*next=get_inode((last)->i_dev,inode_nr);
	printl("??%d\n",(*next)->i_num);
	if((*next)->i_mode!=I_DIRECTORY)
	    return -1;
	put_inode(last);
	return 0;
}

