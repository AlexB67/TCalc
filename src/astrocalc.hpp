#pragma once
#include <math.h>

namespace Astrocalc
{
    struct astrocalc
    {
    public:
        enum { USESTDMETHOD, USESMALLANGLEMETHOD };
        enum { REFLECTOR, REFRACTOR, SCTMAK};
        enum { PLOSSL, ORTHO, MULTIOTHER};
        double calc_tFOV(double, double, double, short) const noexcept;
        double calc_tFOV(double, double) const noexcept;
        double calc_MagA(double, double) const noexcept;
        double calc_MagL(const double flen_s, const double flen_e) const noexcept;
        double calc_kfactor(double) const noexcept;
        double calc_fratio(double, double) const noexcept;
        double calc_airydiskdiam(double, double, double, double) const noexcept;
        double calc_airydiskdiam(double, double, double) const noexcept;
        double calc_PPI(double, double) const noexcept;
        double calc_PPI(double, double, double) const noexcept;
        double calc_lmag_scope(double, double, double, double, double, double ) const noexcept;

        double calc_lmag_scope(int, double, double, double, double, double, double, double, double, double, 
                                double, double, double, short) const noexcept;

        double convert_nelm_to_sqm(double) const noexcept;
        double convert_sqm_to_nelm(double) const noexcept;
        double calc_eyepupil_from_age(double) const noexcept;
        double calc_lunar_res(double) const noexcept;
        double calc_brightness_factor(double, double, double) const noexcept;
        double calc_brightness_factor(double, double, double, double) const noexcept;
        double calc_light_grasp(double aperture, double eyepupil) const noexcept;
        double calc_light_effective_grasp(double aperture, double eyepupil, double obstruct, double mreflect) const noexcept;
        double calc_optimal_minmag(const double aperture) const noexcept;
        double calc_optimal_maxmag(const double aperture) const noexcept;
        double calc_optimal_min_flen(const double aperture, const double flen_s) const noexcept;
        double calc_optimal_max_flen(const double aperture, const double flen_s) const noexcept;
        double calc_maxmag(double) const noexcept;
        double calc_minmag(double, double) const noexcept;
        double calc_Daws_limit(double, double) const noexcept;
        double calc_Rayleigh_limit(double, double, double) const noexcept;
        double calc_exit_pupil(double, double) const noexcept;
        double calc_exit_pupil(double, double, double) const noexcept;
        double calc_minmirrorsize(double, double, double) const noexcept;
        double calc_aff(double, double, double) const noexcept;
        double calc_affl(double, double, double) const noexcept;
        double calc_thrconcs(double, double) const noexcept;
        double calc_thrconls(double, double) const noexcept;
        double calc_contrast_factor(double) const noexcept;
        double calc_ortho_eyerelief(double) const noexcept;
        double calc_plossl_eyerelief(double) const noexcept;
        double calc_dso_mag_to_brightness(const double mag, const double minoraxis, const double majoraxis) const noexcept;
        double calc_contrast_index(const double background_brightness, const double object_brightness) const noexcept;
        double calc_nelm_brightness_threshold_method(const double nelm) const noexcept;
        double calc_nelm_brightness(const double nelm) const noexcept;

        static constexpr double RADTODEG = 57.2957795;
        static constexpr double DEGTOARCMIN = 60.0;
        static constexpr double ARCMINTOARCSEC =  60.0;
        static constexpr double tSMALL = 1.0e-09;
        static constexpr double ETRANSDEFAULT = 96.0;
        static constexpr double ETRANSPLOSSLDEFAULT = 98.0;
        static constexpr double ETRANSORTHODEFAULT = 98.0;
        static constexpr double SCOPEREFLECTDEFAULT = 88.0;
        static constexpr double SCOPETRANSDEFAULT = 99.0;
        static constexpr double OBSTRUCTSIZEDEFAULT = 20.0;
        static constexpr double OBSTRUCTSIZESCTDEFAULT = 33.0;
        
        // for users wanting to override the defaults above you can acces the following parameters
        // they are also initialised to the defaults, but can be overriden.

        inline static double ETRANS = ETRANSDEFAULT;
        inline static double ETRANSPLOSSL = ETRANSPLOSSLDEFAULT;
        inline static double ETRANSORTHO = ETRANSORTHODEFAULT;
        inline static double SCOPEREFLECT = SCOPEREFLECTDEFAULT;
        inline static double SCOPETRANS = SCOPETRANSDEFAULT;
        inline static double OBSTRUCTSIZE = OBSTRUCTSIZEDEFAULT;
        inline static double OBSTRUCTSIZESCT = OBSTRUCTSIZESCTDEFAULT;
    };
}

