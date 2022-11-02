#ifndef LUBAN_C_H
#define LUBAN_C_H

#ifdef __cplusplus
extern "C" {
#endif
void *luban_toolkit_new(const char *config_file);
void luban_toolkit_release(void *ptr);
void luban_entity_array_release(void *ptr);
void *luban_unary_process(void *ptr, char *feature, int feature_len);
void *luban_binary_process(void *ptr, char *featureA, int feature_lenA,
                           char *featureB, int feature_lenB);
#ifdef __cplusplus
} /* end extern "C"*/
#endif

#endif  // LUBAN_LUBAN_C_H
