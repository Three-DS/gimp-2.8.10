#ifndef gimp_composite_altivec_h
#define gimp_composite_altivec_h

extern gboolean gimp_composite_altivec_init (void);

/*
        * The function gimp_composite_*_install() is defined in the code generated by make-install.py
        * I hate to create a .h file just for that declaration, so I do it here (for now).
 */
extern gboolean gimp_composite_altivec_install (void);

#if defined(USE_ALTIVEC)
#if defined(ARCH_PPC)
#if __GNUC__ >= 3
#define COMPILE_ALTIVEC_IS_OKAY (1)
#endif /* __GNUC__ >= 3 */
#endif /* defined(ARCH_PPC) */
#endif /* defined(USE_ALTIVEC) */


#ifdef COMPILE_ALTIVEC_IS_OKAY
extern void gimp_composite_addition_rgba8_rgba8_rgba8_altivec (GimpCompositeContext *ctx);
extern void gimp_composite_subtract_rgba8_rgba8_rgba8_altivec (GimpCompositeContext *ctx);
extern void gimp_composite_swap_rgba8_rgba8_rgba8_altivec (GimpCompositeContext *ctx);
extern void gimp_composite_difference_rgba8_rgba8_rgba8_altivec (GimpCompositeContext *ctx);
extern void gimp_composite_darken_rgba8_rgba8_rgba8_altivec (GimpCompositeContext *ctx);
extern void gimp_composite_lighten_rgba8_rgba8_rgba8_altivec (GimpCompositeContext *ctx);
extern void gimp_composite_multiply_rgba8_rgba8_rgba8_altivec (GimpCompositeContext *ctx);
extern void gimp_composite_blend_rgba8_rgba8_rgba8_altivec (GimpCompositeContext *ctx);
extern void gimp_composite_screen_rgba8_rgba8_rgba8_altivec (GimpCompositeContext *ctx);
extern void gimp_composite_grain_merge_rgba8_rgba8_rgba8_altivec (GimpCompositeContext *ctx);
extern void gimp_composite_grain_extract_rgba8_rgba8_rgba8_altivec (GimpCompositeContext *ctx);
extern void gimp_composite_divide_rgba8_rgba8_rgba8_altivec (GimpCompositeContext *ctx);
extern void gimp_composite_dodge_rgba8_rgba8_rgba8_altivec (GimpCompositeContext *ctx);
#endif /* COMPILE_IS_OKAY */

#endif
