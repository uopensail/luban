#ifndef LUBAN_LUBAN_C_H
#define LUBAN_LUBAN_C_H

#ifdef __cplusplus
extern "C"
{
#endif
    void *luban_new(char *config_file);
    void luban_release(void *ptr);
    void *luban_process(void *ptr, char *feature, int feature_len, void *return_len);
    void *luban_single_process(void *ptr, char *feature, int feature_len, void *return_len);
    void *luban_bicross_process(void *ptr, char *featureA, int feature_lenA,
                                char *featureB, int feature_lenB, void *return_len);
#ifdef __cplusplus
} /* end extern "C"*/
#endif

#endif // LUBAN_LUBAN_C_H
