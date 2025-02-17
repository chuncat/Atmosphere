/*
 * Copyright (c) Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
#include <stratosphere/fs/fs_common.hpp>
#include <stratosphere/fs/impl/fs_newable.hpp>
#include <stratosphere/fs/fsa/fs_ifile.hpp>
#include <stratosphere/fs/fsa/fs_idirectory.hpp>
#include <stratosphere/fs/fsa/fs_ifilesystem.hpp>
#include <stratosphere/fs/common/fs_dbm_hierarchical_rom_file_table.hpp>

namespace ams::fs {

    class RomFsFileSystem : public fsa::IFileSystem, public impl::Newable {
        NON_COPYABLE(RomFsFileSystem);
        public:
            using RomFileTable = HierarchicalRomFileTable;
        private:
            RomFileTable m_rom_file_table;
            IStorage *m_base_storage;
            std::unique_ptr<IStorage> m_unique_storage;
            std::unique_ptr<IStorage> m_dir_bucket_storage;
            std::unique_ptr<IStorage> m_dir_entry_storage;
            std::unique_ptr<IStorage> m_file_bucket_storage;
            std::unique_ptr<IStorage> m_file_entry_storage;
            s64 m_entry_size;
        private:
            Result GetFileInfo(RomFileTable::FileInfo *out, const char *path);
        public:
            static Result GetRequiredWorkingMemorySize(size_t *out, IStorage *storage);
        public:
            RomFsFileSystem();
            virtual ~RomFsFileSystem() override;

            Result Initialize(IStorage *base, void *work, size_t work_size, bool use_cache);
            Result Initialize(std::unique_ptr<IStorage>&& base, void *work, size_t work_size, bool use_cache);

            IStorage *GetBaseStorage();
            RomFileTable *GetRomFileTable();
            Result GetFileBaseOffset(s64 *out, const char *path);
        public:
            virtual Result DoCreateFile(const char *path, s64 size, int flags) override;
            virtual Result DoDeleteFile(const char *path) override;
            virtual Result DoCreateDirectory(const char *path) override;
            virtual Result DoDeleteDirectory(const char *path) override;
            virtual Result DoDeleteDirectoryRecursively(const char *path) override;
            virtual Result DoRenameFile(const char *old_path, const char *new_path) override;
            virtual Result DoRenameDirectory(const char *old_path, const char *new_path) override;
            virtual Result DoGetEntryType(fs::DirectoryEntryType *out, const char *path) override;
            virtual Result DoOpenFile(std::unique_ptr<fs::fsa::IFile> *out_file, const char *path, fs::OpenMode mode) override;
            virtual Result DoOpenDirectory(std::unique_ptr<fs::fsa::IDirectory> *out_dir, const char *path, fs::OpenDirectoryMode mode) override;
            virtual Result DoCommit() override;
            virtual Result DoGetFreeSpaceSize(s64 *out, const char *path) override;
            virtual Result DoGetTotalSpaceSize(s64 *out, const char *path) override;
            virtual Result DoCleanDirectoryRecursively(const char *path) override;

            /* These aren't accessible as commands. */
            virtual Result DoCommitProvisionally(s64 counter) override;
            virtual Result DoRollback() override;
    };

}
