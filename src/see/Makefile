compile =
    g++ - g tree_model_test : tree_model_test.o data.o tree_model.o ${compile} -
    o tree_model_test -
    pthread tree_model_test.o data.o tree_model
        .o

            linear_model_test
    : linear_model_test.o linear_model.o file.o data_io.o data.o ${compile} -
    o linear_model_test linear_model_test.o linear_model.o file.o data_io.o data
        .o

            data.o : data.cc ${compile} -
    c data
        .cc

            linear_model.o : linear_model.cc ${compile} -
    c linear_model
        .cc

            linear_model_test.o : linear_model_test.cc ${compile} -
    c linear_model_test
        .cc

            data_io.o : data_io.cc ${compile} -
    c data_io
            .cc

                file.o :../
        basic / file.cc ${compile} -
    c../ basic /
        file.cc

            tree_model_test.o : tree_model_test.cc ${compile} -
    c tree_model_test
        .cc

            tree_model.o : tree_model.cc ${compile} -
    c tree_model.cc

        clean : rm -
    rf *.o linear_model_test tree_model_test a.out
