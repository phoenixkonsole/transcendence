package=libunbound
$(package)_version=1.9.6
$(package)_download_path=https://github.com/NLnetLabs/unbound/archive/
$(package)_file_name=release-$($(package)_version).tar.gz
$(package)_sha256_hash=12e85421d49564d17918f5430b46787a7ddff36c7b25cfa7538836cf761d8f72
$(package)_dependencies=openssl expat

define $(package)_config_cmds
  $($(package)_autoconf) 
endef

define $(package)_build_cmds
  $(MAKE)
endef

define $(package)_stage_cmds
  $(MAKE) DESTDIR=$($(package)_staging_dir) install
endef