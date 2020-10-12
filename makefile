APP_NAME = main
BUILD_FOLDER = build

$(BUILD_FOLDER)/$(APP_NAME): src/main.c src/ocl_info.c src/ocl_util.c
	mkdir -p $(BUILD_FOLDER)
	gcc $^ -o $@ -lOpenCL -Iinc

#-L/home/den/lib/intel/system_studio_2020/opencl_compilers_and_libraries_18.1.0.013/linux/compiler/lib/intel64_lin


.PHONY: clean
clean:
	rm -rf $(BUILD_FOLDER)
