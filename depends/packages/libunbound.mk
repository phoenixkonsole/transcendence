package=libunbound

$(package)_version=1.10.0
$(package)_download_path=https://github.com/NLnetLabs/unbound/archive/refs/tags/
$(package)_file_name=release-$($(package)_version).tar.gz
$(package)_sha256_hash=F6107A5019EC920F827603AD51550D7024FD03B8274582945ADDAB4A95A90579
$(package)_dependencies=expat zlib openssl
$(package)_patches=fix_socket.patch

define $(package)_config_cmds
  $($(package)_autoconf) 
endef

define $(package)_preprocess_cmds
  patch -p1 -i $($(package)_patch_dir)/fix_socket.patch
endef

define $(package)_build_cmds
  $(MAKE)
endef

define $(package)_stage_cmds
  $(MAKE) DESTDIR=$($(package)_staging_dir) install
endef

define $(package)_set_vars
  $(package)_config_opts=--disable-shared --enable-static-exe --with-pic CXXFLAGS="-fPIC" CPPFLAGS="-fPIC"
endef
