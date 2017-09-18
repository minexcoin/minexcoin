
# test 

package=libsodium

# Identifiers

	$(package)_version=1.0.13
	$(package)_download_path=https://download.libsodium.org/libsodium/releases/
	$(package)_file_name=$(package)-$($(package)_version).tar.gz
	$(package)_sha256_hash=9c13accb1a9e59ab3affde0e60ef9a2149ed4d6e8f99c93c7a5b97499ee323fd


# Build Variables:

    define $(package)_set_vars
    	 $(package)_x86_64_cflags=-O3 -fomit-frame-pointer m64 -mtune=westmere
		 $(package)_i686_cflags=-O3 -fomit-frame-pointer -m32 -march=pentium3 -mtune=westmere
    endef


# Build commands:
	define $(package)_config_cmds
		$($(package)_autoconf) --enable-static --disable-shared
	
endef

	define $(package)_build_cmds
		 $(MAKE)

endef

  define $(package)_stage_cmds
    $(MAKE) DESTDIR=$($(package)_staging_dir) install 

endef
