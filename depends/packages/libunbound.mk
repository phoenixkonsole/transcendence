package=libunbound
$(package)_version=1.10.0
$(package)_download_path=https://github.com/NLnetLabs/unbound/archive/
$(package)_file_name=release-$($(package)_version).tar.gz
$(package)_sha256_hash=F6107A5019EC920F827603AD51550D7024FD03B8274582945ADDAB4A95A90579
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

define $(package)_set_vars
  $(package)_config_opts=--disable-shared --enable-static-exe
endef
