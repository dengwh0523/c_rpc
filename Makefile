all:
	@mkdir -p bin lib
	make -C rmiparse
	make -C librmi_src

clean:
	make -C rmiparse clean
	make -C librmi_src clean
	
