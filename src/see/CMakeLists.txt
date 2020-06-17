cmake_minimum_required(VERSION 3.10) project(see) SET(CMAKE_BUILD_TYPE "Debug")
    add_executable(
        linear_model_test linear_model_test.cc data.cc linear_model.cc../
        basic / file.cc data_io.cc) add_executable(mat_test mat_test.cc../
                                                   basic / forever.cc)
        add_executable(data_test data_test.cc) add_executable(
            tree_model_test tree_model_test.cc data.cc tree_model.cc../ basic /
            file.cc data_io.cc)
            add_executable(word2vec_test word2vec_test.cc word2vec.cc../ basic /
                           file.cc../ basic / forever.cc)
                add_executable(kmeans_test kmeans_test.cc kmeans.cc data.cc)
                    add_executable(dataset_test dataset_test.cc data_io.cc../
                                   basic / file.cc)
                        add_executable(svm_test svm_test.cc svm.cc data
                                           .cc dataset.cc data_io.cc../
                                       basic / file.cc)
                            TARGET_LINK_LIBRARIES(tree_model_test pthread)
                                TARGET_LINK_LIBRARIES(word2vec_test pthread)
                                    TARGET_LINK_LIBRARIES(kmeans_test pthread)
                                        TARGET_LINK_LIBRARIES(svm_test pthread)