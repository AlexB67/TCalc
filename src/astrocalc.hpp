#pragma once
#include <math.h>

namespace Astrocalc
{
struct astrocalc
{
public:
    enum
    {
        USESTDMETHOD,
        USESMALLANGLEMETHOD
    };
    enum
    {
        REFLECTOR,
        REFRACTOR,
        SCTMAK
    };
    enum
    {
        PLOSSL,
        ORTHO,
        MULTIOTHER
    };
    double calc_tFOV(const double, const double, const double, const short) const noexcept;
    double calc_tFOV(const double, const double) const noexcept;
    double calc_MagA(const double, const double) const noexcept;
    double calc_MagL(const double flen_s, const double flen_e) const noexcept;
    double calc_kfactor(const double) const noexcept;
    double calc_fratio(const double, const double) const noexcept;
    double calc_airydiskdiam(const double, const double, const double, const double) const noexcept;
    double calc_airydiskdiam(const double, const double, const double) const noexcept;
    double calc_PPI(const double, const double) const noexcept;
    double calc_PPI(const double, const double, const double) const noexcept;
    double calc_lmag_scope(const double, const double, const double, const double, const double, const double) const noexcept;

    double calc_lmag_scope(const int explevel, const double colindex, const double disksize, const double dirt, const double etrans,
                           const double mreflect, const double aperture, const double eyepupil, const double nelm, const double zenithangle,
                           const double extcoff, const double mag, const double obstruct, const short type) const noexcept;

    double convert_nelm_to_sqm(const double) const noexcept;
    double convert_sqm_to_nelm(const double) const noexcept;
    double calc_eyepupil_from_age(const double) const noexcept;
    double calc_lunar_res(const double) const noexcept;
    double calc_brightness_factor(const double, const double, const double) const noexcept;
    double calc_brightness_factor(const double, const double, const double, const double) const noexcept;
    double calc_light_grasp(const double aperture, const double eyepupil) const noexcept;
    double calc_light_effective_grasp(const double aperture, const double eyepupil, const double obstruct, const double mreflect) const noexcept;
    double calc_optimal_minmag(const double aperture) const noexcept;
    double calc_optimal_maxmag(const double aperture) const noexcept;
    double calc_optimal_min_flen(const double aperture, const double flen_s) const noexcept;
    double calc_optimal_max_flen(const double aperture, const double flen_s) const noexcept;
    double calc_maxmag(const double) const noexcept;
    double calc_minmag(const double, const double) const noexcept;
    double calc_Daws_limit(const double, const double) const noexcept;
    double calc_Rayleigh_limit(const double, const double, const double) const noexcept;
    double calc_exit_pupil(const double, const double) const noexcept;
    double calc_exit_pupil(const double, const double, const double) const noexcept;
    double calc_minmirrorsize(const double, const double, const double) const noexcept;
    double calc_aff(const double, const double, const double) const noexcept;
    double calc_affl(const double, const double, const double) const noexcept;
    double calc_thrconcs(const double, const double) const noexcept;
    double calc_thrconls(const double, const double) const noexcept;
    double calc_contrast_factor(const double) const noexcept;
    double calc_ortho_eyerelief(const double) const noexcept;
    double calc_plossl_eyerelief(const double) const noexcept;
    double calc_dso_mag_to_brightness(const double mag, const double minoraxis, const double majoraxis) const noexcept;
    double calc_contrast_index(const double background_brightness, const double object_brightness) const noexcept;
    double calc_nelm_brightness_threshold_method(const double nelm) const noexcept;
    double calc_nelm_threshold_method(const double surfacebrightness) const noexcept;
    double calc_nelm_brightness(const double nelm) const noexcept;

    std::pair<double, double> calc_dso_contrast_in_scope(const double magnification, const int scopetype, const double aperture,
                                      const double scopeobstruct, const double scopereflect, const double etrans,
                                      const double dirt, const double eyepupilsize, const double nelm, const double dsovmag,
                                      const double minoraxis, const double majoraxis) const noexcept;

    static constexpr double RADTODEG = 57.2957795;
    static constexpr double DEGTOARCMIN = 60.0;
    static constexpr double ARCMINTOARCSEC = 60.0;
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
} // namespace Astrocalc
