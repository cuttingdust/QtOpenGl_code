    #ifndef VERTICES_DATA_H
    #define VERTICES_DATA_H

//    constexpr static float gCubeVertices[] = {
//            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
//            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
//            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
//            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
//            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
//            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
//
//            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
//            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
//            0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
//            0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
//            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
//            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
//
//            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
//            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
//            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
//            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
//            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
//            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
//
//            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
//            0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
//            0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
//            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//
//            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
//            0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
//            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
//            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
//            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
//            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
//
//            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
//            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
//            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
//            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
//            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
//            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f
//    };

   constexpr static float gCubeVertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,// bottom-left
             1.0f,  1.0f, -1.0f,   1.0f, 1.0f,  0.0f,  0.0f, -1.0f,// top-right
             1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,// top-right
            -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,   0.0f, 1.0f,  0.0f,  0.0f, -1.0f,// top-left
            // front face
            -1.0f, -1.0f,  1.0f,   0.0f, 0.0f, 0.0f,  0.0f,  1.0f,// bottom-left
             1.0f, -1.0f,  1.0f,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,// bottom-right
             1.0f,  1.0f,  1.0f,   1.0f, 1.0f, 0.0f,  0.0f,  1.0f,// top-right
             1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 0.0f,  0.0f,  1.0f, // top-right
            -1.0f,  1.0f,  1.0f,   0.0f, 1.0f, 0.0f,  0.0f,  1.0f,// top-left
            -1.0f, -1.0f,  1.0f,   0.0f, 0.0f, 0.0f,  0.0f,  1.0f,// bottom-left
            // left face
            -1.0f,  1.0f,  1.0f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,// top-right
            -1.0f,  1.0f, -1.0f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,// top-left
            -1.0f, -1.0f, -1.0f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,// bottom-left
            -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,// bottom-right
            -1.0f,  1.0f,  1.0f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,// top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,// top-left
             1.0f, -1.0f, -1.0f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,// bottom-right
             1.0f,  1.0f, -1.0f,   1.0f, 1.0f, 1.0f,  0.0f,  0.0f,// top-right
             1.0f, -1.0f, -1.0f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,// bottom-right
             1.0f,  1.0f,  1.0f,   1.0f, 0.0f, 1.0f,  0.0f,  0.0f,// top-left
             1.0f, -1.0f,  1.0f,   0.0f, 0.0f, 1.0f,  0.0f,  0.0f,// bottom-left
            // bottom face
            -1.0f, -1.0f, -1.0f,   0.0f, 1.0f, 0.0f, -1.0f,  0.0f,// top-right
             1.0f, -1.0f, -1.0f,   1.0f, 1.0f, 0.0f, -1.0f,  0.0f,// top-left
             1.0f, -1.0f,  1.0f,   1.0f, 0.0f, 0.0f, -1.0f,  0.0f,// bottom-left
             1.0f, -1.0f,  1.0f,   1.0f, 0.0f, 0.0f, -1.0f,  0.0f,// bottom-left
            -1.0f, -1.0f,  1.0f,   0.0f, 0.0f, 0.0f, -1.0f,  0.0f,// bottom-right
            -1.0f, -1.0f, -1.0f,   0.0f, 1.0f, 0.0f, -1.0f,  0.0f,// top-right
            // top face
            -1.0f,  1.0f, -1.0f,   0.0f, 1.0f, 0.0f,  1.0f,  0.0f,// top-left
             1.0f,  1.0f , 1.0f,   1.0f, 0.0f, 0.0f,  1.0f,  0.0f,// bottom-right
             1.0f,  1.0f, -1.0f,   1.0f, 1.0f, 0.0f,  1.0f,  0.0f,// top-right
             1.0f,  1.0f,  1.0f,   1.0f, 0.0f, 0.0f,  1.0f,  0.0f,// bottom-right
            -1.0f,  1.0f, -1.0f,   0.0f, 1.0f, 0.0f,  1.0f,  0.0f,// top-left
            -1.0f,  1.0f,  1.0f,   0.0f, 0.0f,  0.0f,  1.0f,  0.0f// bottom-left
        };



//    constexpr static float gPlaneVertices[] = {
//        /// positions          /// texcoords       /// normals
//        10.0f, -0.5f, 10.0f, 10.0f, 0.0f, 0.0f, 1.0f, 0.0f,
//        -10.0f, -0.5f, 10.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
//        -10.0f, -0.5f, -10.0f, 0.0f, 10.0f, 0.0f, 1.0f, 0.0f,
//        10.0f, -0.5f, 10.0f, 10.0f, 0.0f, 0.0f, 1.0f, 0.0f,
//        -10.0f, -0.5f, -10.0f, 0.0f, 10.0f, 0.0f, 1.0f, 0.0f,
//        10.0f, -0.5f, -10.0f, 10.0f, 10.0f, 0.0f, 1.0f, 0.0f
//    };

    constexpr static float gPlaneVertices[] = {
        // positions            // normals         // texcoords
         25.0f, -0.5f,  25.0f, 25.0f,  0.0f,  0.0f, 1.0f, 0.0f,
         -25.0f, -0.5f, -25.0f, 0.0f, 25.0f, 0.0f, 1.0f, 0.0f,
         -25.0f, -0.5f,  25.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,

         25.0f, -0.5f,  25.0f, 25.0f,  0.0f, 0.0f, 1.0f, 0.0f,
         25.0f, -0.5f, -25.0f, 25.0f, 25.0f,  0.0f, 1.0f, 0.0f,
         -25.0f, -0.5f, -25.0f,  0.0f, 25.0f, 0.0f, 1.0f, 0.0f
    };

    constexpr static  float gTransparentVertices[] = {
            // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,

    };

    constexpr static  float gGlassVertices[] = {
            // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)

            0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
            5.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
            5.0f, 5.0f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
            5.0f, 5.0f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
            0.0f, 5.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,

    };

    constexpr static float skyboxVertices[] = {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };

    constexpr float gHousePoints[] = {
            -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // top-left
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // top-right
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
            -0.5f, -0.5f, 1.0f, 1.0f, 0.0f // bottom-left
    };

    constexpr float gQuadVertices[] = {
        // positions  // colors
        -0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
        0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
        -0.05f, -0.05f, 0.0f, 0.0f, 1.0f,
        -0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
        0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
        0.05f, 0.05f, 0.0f, 1.0f, 1.0f
    };


    constexpr float gSscreenQuadVertices[] = {
        // positions       //texcoords  // normals
         1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

         1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    };


    constexpr float gTriangleVertices[] = {
        -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,
        0.5f, -0.5f, 0.5f,  0.0f,  0.0f,
        0.0f, 0.5f, 0.5f,  0.0f,  0.0f
    };

    #endif