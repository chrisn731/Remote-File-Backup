all: both

both: client server

client:
	$(MAKE) -C client
	mv client/RBClient ./RBClient
server:
	$(MAKE) -C server
	mv server/RBServer ./RBServer


.PHONY: client server both all
