#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "SuperBlock.h"

f2fs_super_block superBlock;

void readSuperBlock(FILE *file) {
  fseek(file, F2FS_SB1_OFFSET, SEEK_SET);
  fread((void *)&superBlock, sizeof(f2fs_super_block), 1, file);
}

void printSuperBlock() {
  int index;
  printf("-------------------------------------------------------\n");
  printf("Magic Number : 0x%X\n", superBlock.magic);
  printf("Major Version : %d\n", superBlock.major_ver);
  printf("Minor Version : %d\n", superBlock.minor_ver);
  printf("log2 sector size in bytes : %d\n", superBlock.log_sectorsize);
  printf("log2 # of sectors per block : %d\n", superBlock.log_sectors_per_block);
  printf("log2 block size in bytes : %d\n", superBlock.log_blocksize);
  printf("log2 # of blocks per segment : %d\n", superBlock.log_blocks_per_seg);
  printf("# of segments per section : %d\n", superBlock.segs_per_sec);
  printf("# of sections per zone : %d\n", superBlock.secs_per_zone);
  printf("checksum offset inside super block : %d\n", superBlock.checksum_offset);
  printf("total # of user blocks : %ld\n", superBlock.block_count);
  printf("total # of sections : %d\n", superBlock.section_count);
  printf("total # of segments : %d\n", superBlock.segment_count);
  printf("# of segments for checkpoint : %d\n", superBlock.segment_count_ckpt);
  printf("# of segments for SIT : %d\n", superBlock.segment_count_sit);
  printf("# of segments for NAT : %d\n", superBlock.segment_count_nat);
  printf("# of segments for SSA : %d\n", superBlock.segment_count_ssa);
  printf("# of segments for main area : %d\n", superBlock.segment_count_main);
  printf("start block address of segment 0 : 0x%X\n", superBlock.segment0_blkaddr);
  printf("start block address of checkpoint : 0x%X\n", superBlock.cp_blkaddr);
  printf("start block address of SIT : 0x%X\n", superBlock.sit_blkaddr);
  printf("start block address of NAT : 0x%X\n", superBlock.nat_blkaddr);
  printf("start block address of SSA : 0x%X\n", superBlock.ssa_blkaddr);
  printf("start block address of main area : 0x%X\n", superBlock.main_blkaddr);
  printf("root inode number : %d\n", superBlock.root_ino);
  printf("node inode number : %d\n", superBlock.node_ino);
  printf("meta inode number : %d\n", superBlock.meta_ino);
  printf("uuid : ");
  for (index = 0; index < 16; index++)
    printf("%x", superBlock.uuid[index]);
  printf("\n");
  printf("volume name : ");
  for (index = 0; index < MAX_VOLUME_NAME; index++)
    printf("%c", superBlock.volume_name[index]);
  printf("\n");
  printf("# of extensions below : %d\n", superBlock.extension_count);
  printf("extension array : ");
  for (index = 0; index < F2FS_MAX_EXTENSION; index++) {
    if(superBlock.extension_list[index][0] != 0x00)
      printf("%s ", superBlock.extension_list[index]);
  }
  printf("\n");
  printf("the kernel version : %s\n", superBlock.version);
  printf("the initial kernel version : %s\n", superBlock.init_version);
  printf("defined features : 0x%X\n", superBlock.feature);
  printf("quota inode numbers : %ls\n", superBlock.qf_ino);
  printf("# of hot file extension : %d\n", superBlock.hot_ext_count);
  printf("checksum of superblock : %X\n", superBlock.crc);
  printf("-------------------------------------------------------\n");
}
