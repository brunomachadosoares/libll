SUBDIRS	= libll analyzer

all:
	list='$(SUBDIRS)'; \
    for subdir in $$list; do \
    echo -e "\nx-x-x-x-x-x-x-x Making in $$subdir x-x-x-x-x-x-x-x\n"; \
    (cd $$subdir && $(MAKE) all) || exit 1 ; \
    done; echo "Compilado..." \
	
clean:
	list='$(SUBDIRS)'; \
    for subdir in $$list; do \
    echo -e "\nx-x-x-x-x-x-x-x Making in $$subdir x-x-x-x-x-x-x-x\n"; \
    (cd $$subdir && $(MAKE) clean) || exit 1 ; \
    done; echo "Compilado..." \
