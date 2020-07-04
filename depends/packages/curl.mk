package=curl
$(package)_version=7_71_0
$(package)_download_path=https://github.com/curl/curl/archive/
$(package)_file_name=curl-$($(package)_version).tar.gz
$(package)_sha256_hash=46CAE02BA9D12298056FD232D04F4A93149BD7D55B6A5DC80B01A02567D2D0DA
$(package)_dependencies=openssl

define $(package)_config_cmds
  ./buildconf && \
  $($(package)_autoconf) 
endef

define $(package)_build_cmds
  $(MAKE)
endef

define $(package)_stage_cmds
  $(MAKE) DESTDIR=$($(package)_staging_dir) install
endef

define $(package)_set_vars
  $(package)_config_opts=--disable-shared --with-ssl
  $(package)_cflags=-DCURL_STATICLIB
endef
