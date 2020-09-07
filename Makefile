all: both

both: client server

client:
	$(MAKE) quick -C Client
	mv Client/RBClient .
server:
	$(MAKE) quick -C Server
	mv Server/RBServer .


.PHONY: client server both all
