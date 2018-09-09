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

void open_file(struct inode ** dir_inode,struct inode ** pin, char * filename);//父文件夹，所要打开的文件夹（一开始是空的），文件名

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
	if(dir_inode==root_inode)
			printl("from root\n");
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
			if (++m > nr_dir_entries)
				break;
		}
		if (m > nr_dir_entries) /* all entries have been iterated */
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
	const char * s = pathname;
	char * t = filename;

	if (s == 0)
		return -1;

	if (*s == '/')
	{
		s++;
		*ppinode = root_inode;
	}
	else 
	{
		*ppinode = currentDir_inode;
	}
	//在此处解析	

	while (*s) {		/* check each character */
		if (*s == '/')
		{
			*t = 0;
			t=filename;
			open_dir(ppinode,filename);
			memset(filename, 0, MAX_FILENAME_LEN);
			s++;
		}
			
		*t++ = *s++;
		/* if filename is too long, just truncate it */
		if (t - filename >= MAX_FILENAME_LEN)
			break;
	}
	*t = 0;

	return 0;
}

int open_dir(struct inode ** dir_inode,char * filename)
{
	int i,j;
	int inode_nr=0;
	int dir_blk0_nr = (*dir_inode)->i_start_sect;
	printl("sector%d\n",dir_blk0_nr );
	int nr_dir_blks = ((*dir_inode)->i_size + SECTOR_SIZE - 1) / SECTOR_SIZE;
	int nr_dir_entries =
	  (*dir_inode)->i_size / DIR_ENTRY_SIZE; /**
					       * including unused slots
					       * (the file has been deleted
					       * but the slot is still there)
					       */
	int m = 0;
	
	struct dir_entry * pde;
	for (i = 0; i < nr_dir_blks; i++) {
		RD_SECT((*dir_inode)->i_dev, dir_blk0_nr + i);
		pde = (struct dir_entry *)fsbuf;
		printl("search\n");
		for (j = 0; j < SECTOR_SIZE / DIR_ENTRY_SIZE; j++,pde++) {
			printl("%s %d\n",pde->name,pde->inode_nr);
			if (memcmp(filename, pde->name, MAX_FILENAME_LEN) == 0)
				inode_nr= pde->inode_nr;
			if (++m > nr_dir_entries)
				break;
		}
		if (m > nr_dir_entries) /* all entries have been iterated */
			break;
	}
	
	if(inode_nr==0)
	    return -1;
	
	struct inode* pin=0;
	struct inode* old=*dir_inode;
	pin=get_inode((*dir_inode)->i_dev,inode_nr);
	if(pin->i_mode!=I_DIRECTORY)
	    return -1;

	*dir_inode=pin;
	put_inode(old);
	return 0;
}

