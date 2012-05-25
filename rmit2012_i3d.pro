# vim: set expandtab:
                
PRECOMPILED_HEADER = src/precompile.h

CONFIG -= flat
CONFIG += opengl
CONFIG += console

QT = core

TEMPLATE = app
TARGET = 
DEPENDPATH += . \
              src \
              src\app \
              src\engine \
              libs\include\glm \
              src\engine\gameobject \
              libs\include\glm\core \
              libs\include\glm\gtc \
              libs\include\glm\gtx \
              libs\include\glm\virtrev
INCLUDEPATH += .
INCLUDEPATH += libs\include
INCLUDEPATH += src


# Input
HEADERS += src/precompile.h \
           src/app/ship.h \
           src/app/uthash.h \
           src/app/obj.h \
           src/app/water.h \
           src/app/wave.h \
           src/app/manager.h \
           src/app/projectile.h \
           src/engine/application.h \
           src/engine/command.h \
           src/engine/frameeventargs.h \
           src/engine/object.h \
           src/engine/utils.h \
           src/engine/scene.h \
           libs/include/glm/ext.hpp \
           libs/include/glm/glm.hpp \
           src/engine/gameobject/basecomponent.h \
           src/engine/gameobject/basegameobject.h \
           src/engine/gameobject/objectset.h \
           src/engine/gameobject/componentset.h \
           src/engine/gameobject/gameobjectset.h \
           src/engine/gameobject/camera.h\
           src/engine/gameobject/transform.h \
           src/engine/gameobject/rigidbody.h \
           src/engine/gameobject/ball.h \
           libs/include/glm/core/_detail.hpp \
           libs/include/glm/core/_fixes.hpp \
           libs/include/glm/core/_swizzle.hpp \
           libs/include/glm/core/_swizzle_func.hpp \
           libs/include/glm/core/_vectorize.hpp \
           libs/include/glm/core/func_common.hpp \
           libs/include/glm/core/func_exponential.hpp \
           libs/include/glm/core/func_geometric.hpp \
           libs/include/glm/core/func_integer.hpp \
           libs/include/glm/core/func_matrix.hpp \
           libs/include/glm/core/func_noise.hpp \
           libs/include/glm/core/func_packing.hpp \
           libs/include/glm/core/func_trigonometric.hpp \
           libs/include/glm/core/func_vector_relational.hpp \
           libs/include/glm/core/hint.hpp \
           libs/include/glm/core/intrinsic_common.hpp \
           libs/include/glm/core/intrinsic_exponential.hpp \
           libs/include/glm/core/intrinsic_geometric.hpp \
           libs/include/glm/core/intrinsic_matrix.hpp \
           libs/include/glm/core/intrinsic_trigonometric.hpp \
           libs/include/glm/core/intrinsic_vector_relational.hpp \
           libs/include/glm/core/setup.hpp \
           libs/include/glm/core/type.hpp \
           libs/include/glm/core/type_float.hpp \
           libs/include/glm/core/type_gentype.hpp \
           libs/include/glm/core/type_half.hpp \
           libs/include/glm/core/type_int.hpp \
           libs/include/glm/core/type_mat.hpp \
           libs/include/glm/core/type_mat2x2.hpp \
           libs/include/glm/core/type_mat2x3.hpp \
           libs/include/glm/core/type_mat2x4.hpp \
           libs/include/glm/core/type_mat3x2.hpp \
           libs/include/glm/core/type_mat3x3.hpp \
           libs/include/glm/core/type_mat3x4.hpp \
           libs/include/glm/core/type_mat4x2.hpp \
           libs/include/glm/core/type_mat4x3.hpp \
           libs/include/glm/core/type_mat4x4.hpp \
           libs/include/glm/core/type_size.hpp \
           libs/include/glm/core/type_vec.hpp \
           libs/include/glm/core/type_vec1.hpp \
           libs/include/glm/core/type_vec2.hpp \
           libs/include/glm/core/type_vec3.hpp \
           libs/include/glm/core/type_vec4.hpp \
           libs/include/glm/gtc/half_float.hpp \
           libs/include/glm/gtc/matrix_access.hpp \
           libs/include/glm/gtc/matrix_integer.hpp \
           libs/include/glm/gtc/matrix_inverse.hpp \
           libs/include/glm/gtc/matrix_transform.hpp \
           libs/include/glm/gtc/noise.hpp \
           libs/include/glm/gtc/quaternion.hpp \
           libs/include/glm/gtc/random.hpp \
           libs/include/glm/gtc/swizzle.hpp \
           libs/include/glm/gtc/type_precision.hpp \
           libs/include/glm/gtc/type_ptr.hpp \
           libs/include/glm/gtx/associated_min_max.hpp \
           libs/include/glm/gtx/bit.hpp \
           libs/include/glm/gtx/closest_point.hpp \
           libs/include/glm/gtx/color_cast.hpp \
           libs/include/glm/gtx/color_space.hpp \
           libs/include/glm/gtx/color_space_YCoCg.hpp \
           libs/include/glm/gtx/compatibility.hpp \
           libs/include/glm/gtx/component_wise.hpp \
           libs/include/glm/gtx/constants.hpp \
           libs/include/glm/gtx/epsilon.hpp \
           libs/include/glm/gtx/euler_angles.hpp \
           libs/include/glm/gtx/extend.hpp \
           libs/include/glm/gtx/extented_min_max.hpp \
           libs/include/glm/gtx/fast_exponential.hpp \
           libs/include/glm/gtx/fast_square_root.hpp \
           libs/include/glm/gtx/fast_trigonometry.hpp \
           libs/include/glm/gtx/gradient_paint.hpp \
           libs/include/glm/gtx/handed_coordinate_space.hpp \
           libs/include/glm/gtx/inertia.hpp \
           libs/include/glm/gtx/int_10_10_10_2.hpp \
           libs/include/glm/gtx/integer.hpp \
           libs/include/glm/gtx/intersect.hpp \
           libs/include/glm/gtx/log_base.hpp \
           libs/include/glm/gtx/matrix_cross_product.hpp \
           libs/include/glm/gtx/matrix_interpolation.hpp \
           libs/include/glm/gtx/matrix_major_storage.hpp \
           libs/include/glm/gtx/matrix_operation.hpp \
           libs/include/glm/gtx/matrix_query.hpp \
           libs/include/glm/gtx/mixed_product.hpp \
           libs/include/glm/gtx/multiple.hpp \
           libs/include/glm/gtx/noise.hpp \
           libs/include/glm/gtx/norm.hpp \
           libs/include/glm/gtx/normal.hpp \
           libs/include/glm/gtx/normalize_dot.hpp \
           libs/include/glm/gtx/number_precision.hpp \
           libs/include/glm/gtx/ocl_type.hpp \
           libs/include/glm/gtx/optimum_pow.hpp \
           libs/include/glm/gtx/orthonormalize.hpp \
           libs/include/glm/gtx/perpendicular.hpp \
           libs/include/glm/gtx/polar_coordinates.hpp \
           libs/include/glm/gtx/projection.hpp \
           libs/include/glm/gtx/quaternion.hpp \
           libs/include/glm/gtx/random.hpp \
           libs/include/glm/gtx/raw_data.hpp \
           libs/include/glm/gtx/reciprocal.hpp \
           libs/include/glm/gtx/rotate_vector.hpp \
           libs/include/glm/gtx/simd_mat4.hpp \
           libs/include/glm/gtx/simd_vec4.hpp \
           libs/include/glm/gtx/spline.hpp \
           libs/include/glm/gtx/std_based_type.hpp \
           libs/include/glm/gtx/string_cast.hpp \
           libs/include/glm/gtx/transform.hpp \
           libs/include/glm/gtx/transform2.hpp \
           libs/include/glm/gtx/ulp.hpp \
           libs/include/glm/gtx/unsigned_int.hpp \
           libs/include/glm/gtx/vec1.hpp \
           libs/include/glm/gtx/vector_access.hpp \
           libs/include/glm/gtx/vector_angle.hpp \
           libs/include/glm/gtx/vector_query.hpp \
           libs/include/glm/gtx/verbose_operator.hpp \
           libs/include/glm/gtx/wrap.hpp \
           libs/include/glm/virtrev/xstream.hpp \
           libs/include/glm/core/type_half.inl \
           libs/include/glm/core/type_vec1.inl \
           libs/include/glm/core/type_vec2.inl \
           libs/include/glm/core/type_vec3.inl \
           libs/include/glm/core/type_vec4.inl \
           libs/include/glm/core/type_mat2x2.inl \
           libs/include/glm/core/type_mat2x3.inl \
           libs/include/glm/core/type_mat2x4.inl \
           libs/include/glm/core/type_mat3x2.inl \
           libs/include/glm/core/type_mat3x3.inl \
           libs/include/glm/core/type_mat3x4.inl \
           libs/include/glm/core/type_mat4x2.inl \
           libs/include/glm/core/type_mat4x3.inl \
           libs/include/glm/core/type_mat4x4.inl \
           libs/include/glm/core/func_trigonometric.inl \
           libs/include/glm/core/func_exponential.inl \
           libs/include/glm/core/func_common.inl \
           libs/include/glm/core/func_packing.inl \
           libs/include/glm/core/func_geometric.inl \
           libs/include/glm/core/func_matrix.inl \
           libs/include/glm/core/func_vector_relational.inl \
           libs/include/glm/core/func_integer.inl \
           libs/include/glm/core/func_noise.inl \
           libs/include/glm/gtc/half_float.inl \
           libs/include/glm/gtc/matrix_access.inl \
           libs/include/glm/gtc/matrix_inverse.inl \
           libs/include/glm/gtc/matrix_transform.inl \
           libs/include/glm/gtc/quaternion.inl \
           libs/include/glm/gtc/type_precision.inl \
           libs/include/glm/gtc/swizzle.inl \
           libs/include/glm/gtc/type_ptr.inl \
           libs/include/glm/gtx/associated_min_max.inl \
           libs/include/glm/gtx/bit.inl \
           libs/include/glm/gtx/closest_point.inl \
           libs/include/glm/gtx/number_precision.inl \
           libs/include/glm/gtx/color_cast.inl \
           libs/include/glm/gtx/color_space.inl \
           libs/include/glm/gtx/color_space_YCoCg.inl \
           libs/include/glm/gtx/compatibility.inl \
           libs/include/glm/gtx/component_wise.inl \
           libs/include/glm/gtx/epsilon.inl \
           libs/include/glm/gtx/euler_angles.inl \
           libs/include/glm/gtx/extend.inl \
           libs/include/glm/gtx/extented_min_max.inl \
           libs/include/glm/gtx/fast_exponential.inl \
           libs/include/glm/gtx/fast_square_root.inl \
           libs/include/glm/gtx/fast_trigonometry.inl \
           libs/include/glm/gtx/optimum_pow.inl \
           libs/include/glm/gtx/gradient_paint.inl \
           libs/include/glm/gtx/handed_coordinate_space.inl \
           libs/include/glm/gtx/inertia.inl \
           libs/include/glm/gtx/raw_data.inl \
           libs/include/glm/gtx/int_10_10_10_2.inl \
           libs/include/glm/gtx/integer.inl \
           libs/include/glm/gtx/intersect.inl \
           libs/include/glm/gtx/log_base.inl \
           libs/include/glm/gtx/matrix_cross_product.inl \
           libs/include/glm/gtx/matrix_interpolation.inl \
           libs/include/glm/gtx/matrix_major_storage.inl \
           libs/include/glm/gtx/matrix_operation.inl \
           libs/include/glm/gtx/vector_query.inl \
           libs/include/glm/gtx/matrix_query.inl \
           libs/include/glm/gtx/mixed_product.inl \
           libs/include/glm/gtx/multiple.inl \
           libs/include/glm/gtx/noise.inl \
           libs/include/glm/gtx/norm.inl \
           libs/include/glm/gtx/normal.inl \
           libs/include/glm/gtx/normalize_dot.inl \
           libs/include/glm/gtx/ocl_type.inl \
           libs/include/glm/gtx/orthonormalize.inl \
           libs/include/glm/gtx/projection.inl \
           libs/include/glm/gtx/perpendicular.inl \
           libs/include/glm/gtx/polar_coordinates.inl \
           libs/include/glm/gtx/random.inl \
           libs/include/glm/gtx/reciprocal.inl \
           libs/include/glm/gtx/transform.inl \
           libs/include/glm/gtx/rotate_vector.inl \
           libs/include/glm/gtx/spline.inl \
           libs/include/glm/gtx/std_based_type.inl \
           libs/include/glm/gtx/string_cast.inl \
           libs/include/glm/gtx/transform2.inl \
           libs/include/glm/gtx/ulp.inl \
           libs/include/glm/gtx/vec1.inl \
           libs/include/glm/gtx/vector_access.inl \
           libs/include/glm/gtx/vector_angle.inl \
           libs/include/glm/gtx/verbose_operator.inl \
           libs/include/glm/gtx/wrap.inl \
           libs/include/glm/core/intrinsic_common.inl \
           libs/include/glm/core/intrinsic_geometric.inl \
           libs/include/glm/gtx/simd_vec4.inl \
           libs/include/glm/core/intrinsic_matrix.inl \
           libs/include/glm/gtx/simd_mat4.inl
SOURCES += src/main.cpp \
           src/precompile.cpp \
           src/app/ship.cpp \
           src/app/water.cpp \
           src/app/wave.cpp \
           src/app/manager.cpp \
           src/app/obj.cpp \
           src/app/projectile.cpp \
           src/engine/application.cpp \
           src/engine/command.cpp \
           src/engine/frameeventargs.cpp \
           src/engine/utils.cpp \
           src/engine/scene.cpp \
           src/engine/gameobject/componentset.cpp \
           src/engine/gameobject/gameobjectset.cpp \
           src/engine/gameobject/objectset.cpp \
           src/engine/gameobject/camera.cpp \
           src/engine/gameobject/basecomponent.cpp \
           src/engine/gameobject/basegameobject.cpp \
           src/engine/gameobject/transform.cpp \
           src/engine/gameobject/rigidbody.cpp \
           src/engine/gameobject/ball.cpp \

win32-msvc2010:LIBS += SDL.lib
