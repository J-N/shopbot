
all: libcreateoi navigation

libcreateoi: force_look
	$(MAKE) -C libcreateoi
	$(MAKE) install -C libcreateoi

navigation: libcreateoi
	$(MAKE) -C src

force_look:
	true

clean:
	$(MAKE) clean -C libcreateoi
	$(MAKE) clean -C src
