SRC_NAME=mdnsd-1.1
SRC_URL=https://github.com/troglobit/mdnsd/releases/download/v1.1/mdnsd-1.1.tar.gz

THISDIR = $(shell pwd)

CFLAGS += -fno-unwind-tables -fno-asynchronous-unwind-tables

all: download_test extract_test config_test
	$(MAKE) -j$(HOST_NCPU) -C $(SRC_NAME)
	$(MAKE) -j$(HOST_NCPU) -C $(SRC_NAME) install DESTDIR=""

download_test:
	( if [ ! -f $(THISDIR)/$(SRC_NAME).tar.gz ]; then \
		wget -t5 --timeout=20 --no-check-certificate -O $(SRC_NAME).tar.gz $(SRC_URL); \
	fi )

extract_test:
	( if [ ! -d $(THISDIR)/$(SRC_NAME) ]; then \
		tar zxf $(SRC_NAME).tar.gz; \
	fi )

config_test:
	( if [ -f ./config_done ]; then \
		echo "the same configuration"; \
	else \
		make clean;\
		make configure && touch config_done; \
	fi )

configure:
	( cd $(SRC_NAME) ; \
		./configure \
                --prefix=$(STAGEDIR) \
                --host=$(HOST_TARGET) \
                --build=$(HOST_BUILD) 	;\
	)

clean:
	#rm -rf $(SRC_NAME)/build
	rm -f config_done
	make -C $(SRC_NAME) clean

romfs:
	$(ROMFSINST) $(STAGEDIR)/sbin/mdnsd /sbin/mdnsd
	$(ROMFSINST) $(STAGEDIR)/bin/mquery /bin/mquery
