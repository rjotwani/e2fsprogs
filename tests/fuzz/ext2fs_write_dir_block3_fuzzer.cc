// Copyright 2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

#include "ext2fs/ext2fs.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  static const char* pattern = "/dev/shm/ext2XXXXXX";
  int fd;
  char* fname;

  // Write our data to a temp file.
  fname = strdup(pattern);
  fd = mkstemp(fname);
  write(fd, data, size);
  close(fd);

  ext2_filsys fs;
  errcode_t retval = ext2fs_open(
      fname,
      0, 0, 0,
      unix_io_manager,
      &fs);

  if (!retval) {
    void *buf;
    retval = ext2fs_write_dir_block3(fs, 0, buf, 0);
    ext2fs_close(fs);
  }

  unlink(fname);
  free(fname);
  return 0;
}
