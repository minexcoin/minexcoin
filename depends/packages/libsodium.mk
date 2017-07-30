
# test 

package=libsodium

# Identifiers

	$(package)_version=1.0.8
	$(package)_download_path=https://download.libsodium.org/libsodium/releases/old
	$(package)_file_name=$(package)-$($(package)_version).tar.gz
	$(package)_sha256_hash=c0f191d2527852641e0a996b7b106d2e04cbc76ea50731b2d0babd3409301926

# Build Variables:

    define $(package)_set_vars
    	 $(package)_x86_64_cflags=-O3 -fomit-frame-pointer m64 -mtune=westmere
		 $(package)_i686_cflags=-O3 -fomit-frame-pointer -m32 -march=pentium3 -mtune=westmere
    endef


# Build commands:
	define $(package)_config_cmds
	  	./configure  --prefix=$(host_prefix)  --exec-prefix=$(host_prefix) --host=$(host) 
	
endef

	define $(package)_build_cmds
		make -j 8 && make check
	
endef

  define $(package)_stage_cmds
    $(MAKE) DESTDIR=$($(package)_staging_dir) install 

endef
