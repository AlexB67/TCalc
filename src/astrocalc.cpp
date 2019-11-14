#include "astrocalc.hpp"
#include <array>

using namespace Astrocalc;

double astrocalc::calc_tFOV(const double flen_s, const double fstop) const noexcept
{

    // returns tFOV in arc minutes using either small angle approximation
    // or exact angle method using the field stop formula
    // INPUTS: focal length of telescope field stop data, method ( exact or small angle approximation )
    // returns tFOV

    return fabs(flen_s) < tSMALL ? 0.0 : DEGTOARCMIN * RADTODEG * 2.0 * atan(fstop / (2.0 * flen_s));
}

double astrocalc::calc_tFOV(const double aFOV, const double flen_s, const double flen_e, const short method) const noexcept
{

    // return tFOV in arc minutes
    // aFOV assumed to be in degrees.
    // two options, use the small angle approximation or exact method.

    if (fabs(flen_s) < tSMALL)
        return 0;

    if (method == USESTDMETHOD)
    {
        double dtFOV = DEGTOARCMIN * RADTODEG * 2.0 * atan(tan(aFOV / (2.0 * RADTODEG)) * flen_e / flen_s);
        return dtFOV;
    }
    else
    {
        double dtFOV = DEGTOARCMIN * aFOV * flen_e / flen_s;
        return dtFOV;
    }
}

double astrocalc::calc_MagL(const double flen_s, const double flen_e) const noexcept
{

    // small angle approximation to calculate mag.
    // INPUTS: focal length of telescope and eyepiece respectively
    // OUTPUTS: none
    // RETURNS: Linear magnification

    if (fabs(flen_e) < tSMALL)
        return 0.0;

    return flen_s / flen_e;
}

double astrocalc::calc_MagA(const double aFOV, const double dtFOV) const noexcept
{

    // calculate mag using angle definition.
    // INPUTS: aFOV, dtFOV apperant and true angle of view
    // in degrees
    // OUTPUTS: none
    // RETURNS: Angular magnification

    if (dtFOV < tSMALL)
        return 0.0;

    // http://www.saharasky.com/saharasky/formula.html#1

    return tan(aFOV / (2 * RADTODEG)) / tan(dtFOV / (2 * RADTODEG));
}

double astrocalc::calc_kfactor(const double aFOV) const noexcept
{
    // as defined in Journal of the British Astronomical Association, vol.105, no.5, p.242-245
    // returns the k factor for a given FOV
    // INPUTS: aFOV in degrees
    // OUTPUTS: none
    // RETURNS: k factor

    return 2 * RADTODEG * tan(aFOV / (2.0 * RADTODEG));
}

double astrocalc::calc_fratio(const double aperture, const double flen_s) const noexcept
{
    // return the  fratio (speed) of a telescope
    // INPUTS: telescope aperture and focla length
    // OUTPUTS: none
    // RETURNS: fratio
    if (fabs(aperture) < tSMALL)
        return 0.0;

    return flen_s / aperture;
}
double astrocalc::calc_airydiskdiam(const double dlambda, const double flen_s, const double aperture, const double obstruct) const noexcept
{
    // INPUTS: wavelength in nanometers
    // aperture, focal length and aperture any units, but must be the same
    // OUTPUT: none
    // RETURNS: returns the airy disk diameter in mm
    // The obstruction dependence uses a series expansion
    // reasonable approx for obstructions < 0.4

    if (fabs(aperture) < tSMALL)
        return 0.0;

    return (1 - obstruct * obstruct) * 2.43932 * dlambda * 1E-06 * flen_s / aperture;
}

double astrocalc::calc_airydiskdiam(const double dlambda, const double aperture, const double obstruct) const noexcept
{
    // INPUTS: wavelength in nanometer,  aperture in mm, obstruction size (as a fraction)
    // OUTPUT: none
    // RETURNS: returns the angular airy disk diameter in arc seconds
    // The obstruction dependence uses a series expansion
    // reasonble approx for obstructions < 0.4

    return fabs(aperture) < tSMALL ? 0.0 : ((1 - obstruct * obstruct)) * 7200 * atan(RADTODEG * 1.21966 * dlambda * 1E-06 / aperture);
}

double astrocalc::calc_PPI(const double dMag, const double aperture) const noexcept
{
    // INPUTS: aperture in mm
    // OUTPUT: none
    // RETURNS: returns the PPI value of the scope.

    if (fabs(aperture) < tSMALL)
        return 0.0;

    return 25.40 * dMag / aperture;
}

double astrocalc::calc_PPI(const double flen_s, const double flen_e, const double aperture) const noexcept
{
    // INPUTS: focal length of scope and eyepiece
    // OUTPUT: none
    // RETURNS: returns the PPI value of the scope.

    if (fabs(aperture) < tSMALL || fabs(flen_e) < tSMALL)
        return 0.0;

    return 25.40 * (flen_s / flen_e) / aperture;
}

double astrocalc::calc_lmag_scope(const double transcof, const double aperture, const double eyepupil, const double nelm,
                                  const double exitpupil, const double zenithangle) const noexcept
{
    if (eyepupil < tSMALL || fabs(90 - zenithangle) < tSMALL)
        return 0.0;

    // cummulative factor for now to allow for light loss and secondary obstruction plus ep optics
    // newtonian 2 surfaces, 2 mirrors + eyepiece.
    // Though crude, agrees quite well with http://www.cruxis.com/scope/limitingmagnitude.htm

    double dstarmaglimit = 5.0 * log10(sqrt(transcof) * aperture / eyepupil) + nelm;
    dstarmaglimit += 2.0 * log10(eyepupil / exitpupil);
    // atmospheric effects due to zenith angle
    dstarmaglimit -= (0.25 + 0.17 * zenithangle / (90 - zenithangle));

    return dstarmaglimit;
}

double astrocalc::calc_lmag_scope(const int explevel, const double colindex, const double disksize, const double dirt, const double etrans,
                                  const double mreflect, const double aperture, const double eyepupil, const double nelm, const double zenithangle,
                                  const double extcoff, const double mag, const double obstruct, const short type) const noexcept
{
    // direct translation from SKY & TELESCOPE magazine, November, 1989, page 522. Schaefer.
    // INPUTS
    // dirt: Measure of optical cleanliness 0 = 100 % 0.15 srtandard, clean 0.3 dirty
    // etrans: transmission due to eyepiece
    // mreflect: reflectivity of mirror
    // disksize: ( diameter ) of object in arc sec
    // colindex: colour index of star B - V scale

    const double m_zenithangle = zenithangle / RADTODEG;
    double m_etrans = etrans;

    double exitpupil = 0.0;
    constexpr double cc = 1.58E-10; // night-vision constant
    constexpr double kk = 0.0126;   // night-vision constant
    double fd = 0;
    double fl = 0;

    if (REFLECTOR == type) // reflectors
    {
        fl = pow(mreflect, 2);       // reflectivity effciency due to primary and secondary.
        fd = 1.0 - pow(obstruct, 2); // net effect due to obstruction.
    }

    if (REFRACTOR == type) // refractors
    {
        fd = 1.0;
        fl = pow(mreflect, 4);
    }

    if (SCTMAK == type)
    {
        fd = 1.0 - pow(obstruct, 2);
        fl = pow((SCOPETRANS / 100.0) * mreflect, 2); // SCT
    }

    if (REFLECTOR != type)
        m_etrans = m_etrans * 0.97; // we'll assume the added diagonal has 97% reflectivity

    m_etrans *= 1 - dirt;                   // effect of dirt on transmission
    double ft = 1.0 / (fl * fd * m_etrans); // net telescope transmission

    // Compute sky brightness from NELM in zenith

    double fs = 1.0;          // observer's eyesight
    double k = 1.2 * extcoff; // extinction corrected to 510 nm (averted vision)
    double mzz = nelm + k;    // zenith NELM without atmosphere
    double bs;
    if (mzz >= 7)
    {
        bs = 54;                         // best possible sky brightness => Ba = 21.58
        fs = pow(10, 0.4 * (7.0 - mzz)); // good eyesight correction
    }
    else
    {
        double xx = 0.2 * (8.68 - mzz);        //  fs ASSUMED = 1  ; 8.68 = 7.93 + 0.75
        bs = 39.7 * pow((pow(10, xx) - 1), 2); // SKY BRIGHTNESS FOR MZ
    }

    double fb = sqrt(2); // binocular vision to monocular correction
    double fp = 1.0;

    exitpupil = calc_exit_pupil(aperture, mag);
    if (eyepupil < exitpupil) // light outside pupil
        fp = pow(exitpupil / eyepupil, 2);

    double fa = pow(eyepupil / aperture, 2);           // light collecting area
    double fm = mag * mag;                             // spread of sky photons
    double fc = pow(10, 0.4 * (colindex / 2 - 1));     // colour of star
    bs *= 1.0 + 0.5 * m_zenithangle * m_zenithangle;   // correction for zenith distance
    double b = bs / (fb * ft * fp * fa * fm * fc);     // background brightness in telescope
    double fe = pow(10, 0.4 * k / cos(m_zenithangle)); // atmospheric extinction
    double theta = disksize * mag;                     // apparent size of seeing disk in arc sec.

    double fr = 1.0; // point source extended
    if (theta > 900)
        fr = sqrt(theta / 900);

    double i = cc * pow(1 + sqrt(kk * b), 2);
    double is = i * fb * fe * ft * fp * fa * fr * fc * fs; // for no scope, no air
    double lmag = -16.57 - 2.5 * log10(is);
    lmag += (static_cast<double>(explevel) - 6) * 0.16; // experience level.
    return lmag;
}

double astrocalc::convert_nelm_to_sqm(const double nelm) const noexcept
{

    // qreal dval = 21.58 - 5 * log10(pow(10, 1.586 - nelmvalue->value() / 5) - 1); //schaefer
    // inverse function of calc_nelm_value, condition: log arg > 0.
    // return 0.0 when factor < 0 else sqm

    double sqm = -3.16222E-05 * (1.08416 - 59.0328 * exp(-0.460517 * nelm));
    return sqm < 0 ? 0.0 : -2.16934 * log(sqm);
}

double astrocalc::convert_sqm_to_nelm(const double sqm) const noexcept
{
    // qreal dval = 7.93 - 5 * log10(pow(10, 4.316 - sqmvalue->value() / 5) + 1); // schaefer
    // nelmvalue->setValue(dval);
    // nelmvalue->setValue((sqmvalue->value()-8.89)/2 + 0.5);  yet another method

    return 8.68 - 5 * log10(1 + 0.158 * sqrt(34.08 * exp(20.7233 - 0.92194 * sqm)));
}

double astrocalc::calc_eyepupil_from_age(const double age) const noexcept
{
    double pupilsize = 7.0 * exp(-age * age / 20000.0);
    return fabs(pupilsize) > 4 ? pupilsize : 4.0;
}

double astrocalc::calc_lunar_res(const double aperture) const noexcept
{
    // INPUTS: aperture in mm
    // OUTPUTS: None
    // returns lunar resolution or 0.0 if eperture is 0

    return fabs(aperture) < tSMALL ? 0.0 : 447.24 / aperture;
}

double astrocalc::calc_brightness_factor(const double aperture, const double eyepupil, const double mag) const noexcept
{
    // INPUTS: aperture eyepupil in the same units
    // OUTPUTS: None
    // returns brightness factor

    return fabs(eyepupil) < tSMALL ? 0.0 : pow((aperture / eyepupil) / mag, 2);
}

double astrocalc::calc_brightness_factor(const double aperture, const double eyepupil, const double flen_s, const double flen_e) const noexcept
{
    // INPUTS: aperture eyepupil in the same units
    // OUTPUTS: None
    // returns brightness factor

    return fabs(eyepupil < tSMALL || flen_e < tSMALL) ? 0.0 : pow((aperture / eyepupil) / (flen_s / flen_e), 2);
}

double astrocalc::calc_light_grasp(const double aperture, const double eyepupil) const noexcept
{
    // INPUTS: aperture eyepupil in the same units
    // OUTPUTS: None
    // returns ligth grasp

    return fabs(eyepupil) < tSMALL ? 0.0 : pow(aperture / eyepupil, 2);
}
double astrocalc::calc_light_effective_grasp(const double aperture, const double eyepupil, const double obstruct, const double mreflect) const noexcept
{
    // INPUTS: aperture eyepupil in the same units
    // OUTPUTS: None
    // returns effective light grasp accounting for light loss, central obstruction + reflectivity.

    double m_aperture = aperture;
    m_aperture = aperture * sqrt(1.0 - obstruct * obstruct) * mreflect;

    return fabs(eyepupil) < tSMALL ? 0.0 : pow(m_aperture / eyepupil, 2);
}

double astrocalc::calc_optimal_minmag(const double aperture) const noexcept
{
    // INPUTS: aperture in mm
    // OUTPUTS: None
    // RETURNS: optimal minimum magnification: eyepupil size 4 mm
    return aperture / 4.0;
}

double astrocalc::calc_optimal_min_flen(const double aperture, const double flen_s) const noexcept
{
    // INPUTS: aperture in mm
    // OUTPUTS: None
    // RETURNS: optimal minimum magnification: eyepupil size 2 mm

    if (aperture < tSMALL)
        return 0;

    return flen_s / (aperture / 2.0);
}

double astrocalc::calc_optimal_maxmag(const double aperture) const noexcept
{
    // INPUTS: aperture in mm
    // OUTPUTS: None
    // RETURNS: optimal maximum magnification: eyepupil size 2 mm

    return aperture / 2.0;
}

double astrocalc::calc_optimal_max_flen(const double aperture, const double flen_s) const noexcept
{
    // INPUTS: aperture in mm
    // OUTPUTS: None
    // RETURNS: optimal maximum magnification: eyepupil size 4 mm

    if (aperture < tSMALL)
        return 0;

    return flen_s / (aperture / 4.0);
}

double astrocalc::calc_maxmag(const double aperture) const noexcept
{
    // INPUTS: aperture in mm
    // returns the highest magnification where the telescope
    // optical resolution is not limited
    //
    return 1.9685 * aperture;
}

double astrocalc::calc_minmag(const double aperture, const double eyepupil) const noexcept
{
    // INPUTS: aperture in mm
    // returns the lowest magnification where eye epupil size = exit pupil
    //
    return fabs(eyepupil) < tSMALL ? 0.0 : aperture / eyepupil;
}

double astrocalc::calc_Daws_limit(const double aperture, const double lambda) const noexcept
{
    // INPUTS: aperture in mm
    // wavelenghth in nm.
    // returns Daw's limit.

    return fabs(aperture) < tSMALL ? 0.0 : 210589.0 * lambda * 1E-06 / aperture;
}

double astrocalc::calc_Rayleigh_limit(const double aperture, const double lambda, const double obstruct) const noexcept
{
    return 0.5 * calc_airydiskdiam(lambda, aperture, obstruct);
}

double astrocalc::calc_exit_pupil(const double aperture, const double mag) const noexcept
{
    // INPUTS: aperture in mm
    // returns exit pupil
    return fabs(mag) < tSMALL ? 0.0 : aperture / mag;
}
double astrocalc::calc_exit_pupil(const double aperture, const double flen_s, const double flen_e) const noexcept
{
    // INPUTS: aperture in mm
    // returns exit pupil
    return fabs(flen_s) < tSMALL ? 0.0 : aperture * flen_s * flen_e / (flen_s * flen_s - flen_e * flen_e);
}

double astrocalc::calc_minmirrorsize(const double aperture, const double flen_s, const double fplane_s) const noexcept
{
    // INPUTS: aperture in mm, focal length, dist from secondary to focal plane
    // return minimum obstruction diamteer of secondary as a percentage of the primary diameter.

    double fratio = calc_fratio(aperture, flen_s);

    if (fratio < tSMALL)
        return 0.0;

    return 100.0 * fplane_s * fratio / aperture;
}
double astrocalc::calc_aff(const double aperture, const double flen_s, const double lambda) const noexcept
{
    double fratio = calc_fratio(aperture, flen_s);

    if (fratio < tSMALL || flen_s < tSMALL)
        return 0.0;

    double factor = 9.0 / (256 * fratio * fratio) + fratio * 1E-06 * 2.43932 * lambda / aperture;

    if (factor < tSMALL)
        return 0.0;

    return 2 * 60 * fabs(-3.0 / (16.0 * fratio) + sqrt(factor)) * RADTODEG;
}

double astrocalc::calc_affl(const double aperture, const double flen_s, double lambda) const noexcept
{
    return flen_s * calc_aff(aperture, flen_s, lambda) / (60 * RADTODEG);
}

double astrocalc::calc_contrast_factor(const double obstruct) const noexcept
{
    // emperical formula for CF 5.25 - 5.1x - 34.1x 2 + 51.1x 3
    return (5.25 - 5.1 * obstruct - 34.1 * pow(obstruct, 2) + 51.1 * pow(obstruct, 3));
}

double astrocalc::calc_dso_mag_to_brightness(const double mag, const double minoraxis, const double majoraxis) const noexcept
{
    //  return ( mag + (2.5 * (log10(2827 * majoraxis * minoraxis))));
    return (mag + (2.5118 * (log10(2827.4334 * majoraxis * minoraxis))));
}

double astrocalc::calc_nelm_brightness(const double nelm) const noexcept
{
    return (21.58 - 5.0 * log10(pow(10, (1.586 - nelm / 5)) - 1.0));
}

double astrocalc::calc_nelm_brightness_threshold_method(const double nelm) const noexcept
{
    // as dicussed in https://www.uv.es/jrtorres/visib.html

    return (24.19 - 2.814 * nelm + 0.3694 * nelm * nelm);
}

double astrocalc::calc_nelm_threshold_method(const double surfacebrightness) const noexcept
{
    // as dicussed in https://www.uv.es/jrtorres/visib.html

    return (2.814 + sqrt(2.814 * 2.814 - 4 * 0.3691 * (24.19 - surfacebrightness))) / (2 * 0.3694);
}

double astrocalc::calc_contrast_index(const double background_brightness, const double object_brightness) const noexcept
{
    // rounded to return (-0.40* (object_brightness - background_brightness));
    return (-0.3981 * (object_brightness - background_brightness));
}

double astrocalc::calc_ortho_eyerelief(const double e_flen) const noexcept
{
    // emperical formula for genuine ortho eye relief
    return (0.849386 * e_flen - 0.0966205);
}

double astrocalc::calc_plossl_eyerelief(const double e_flen) const noexcept
{
    // emperical formula for genuine plossl eye relief
    return (0.687131 * e_flen + 0.181371);
}

double astrocalc::calc_thrconcs(const double angle, const double sb) const noexcept
{

    // a more precise interpolation of the Clark surface

    static constexpr std::array<double, 24> angx = {-0.225, -0.1042, 0.0166, 0.1374, 0.2582, 0.379, 0.4998, 0.6206, 0.7414, 0.8622, 0.983, 1.1038, 1.2246, 1.3454, 1.4662, 1.587, 1.7078, 1.8286, 1.9494, 2.0702, 2.191, 2.3118, 2.4326, 2.5534};
    static constexpr std::array<std::array<double, 24>, 24> sbx =
     {{{-0.377887, -0.613249, -0.846476, -1.07545, -1.29803, -1.5121, -1.71554, -1.90607, -2.07828, -2.22393, -2.33467, -2.40598, -2.44996, -2.48218, -2.50845, -2.52895, -2.54377, -2.5531, -2.55817, -2.56077, -2.5625, -2.56385, -2.56496, -2.56595},
          {-0.332424, -0.567304, -0.800434, -1.03007, -1.25447, -1.47189, -1.68059, -1.87861, -2.0598, -2.21412, -2.33141, -2.40589, -2.45064, -2.48277, -2.50874, -2.52888, -2.5434, -2.55264, -2.55779, -2.56052, -2.56232, -2.5637, -2.56479, -2.56573},
          {-0.269157, -0.504273, -0.738128, -0.969468, -1.19704, -1.41958, -1.63584, -1.84428, -2.03762, -2.20331, -2.32861, -2.40602, -2.45059, -2.48221, -2.50819, -2.52863, -2.54349, -2.55289, -2.55799, -2.56062, -2.56236, -2.56372, -2.56483, -2.56581},
          {-0.199155, -0.435263, -0.670433, -0.903735, -1.13423, -1.361, -1.58309, -1.79926, -2.0018, -2.17711, -2.31135, -2.39608, -2.44608, -2.48117, -2.50899, -2.53008, -2.54493, -2.55408, -2.55893, -2.56134, -2.56296, -2.56429, -2.56541, -2.56644},
          {-0.124798, -0.362056, -0.598537, -0.833467, -1.06607, -1.29558, -1.52121, -1.74187, -1.95014, -2.13276, -2.27627, -2.37206, -2.43256, -2.47483, -2.50624, -2.5285, -2.54328, -2.55222, -2.55704, -2.5595, -2.56115, -2.56247, -2.56356, -2.56455},
          {-0.0434279, -0.281032, -0.518016, -0.753761, -0.987649, -1.21906, -1.44738, -1.67168, -1.88487, -2.07418, -2.22663, -2.3336, -2.40502, -2.45513, -2.49095, -2.51534, -2.53106, -2.54084, -2.54661, -2.54987, -2.55197, -2.55341, -2.55441, -2.55518},
          {0.0488622, -0.187784, -0.424062, -0.659605, -0.894047, -1.12702, -1.35816, -1.58678, -1.80577, -2.00213, -2.16264, -2.27825, -2.35777, -2.41425, -2.45463, -2.48234, -2.50074, -2.51308, -2.52127, -2.52646, -2.52972, -2.53166, -2.5327, -2.5333},
          {0.158586, -0.0763218, -0.311168, -0.545893, -0.780436, -1.01474, -1.24873, -1.48201, -1.70717, -1.91037, -2.07755, -2.19898, -2.28365, -2.34493, -2.38992, -2.42207, -2.44476, -2.46125, -2.47324, -2.48157, -2.48706, -2.49045, -2.49242, -2.49368},
          {0.292389, 0.0592546, -0.174112, -0.407941, -0.642464, -0.877912, -1.11452, -1.35212, -1.58286, -1.79183, -1.96382, -2.08841, -2.17569, -2.24053, -2.29027, -2.32788, -2.35629, -2.3783, -2.39533, -2.40806, -2.41719, -2.42357, -2.42811, -2.43172},
          {0.454775, 0.221841, -0.0114456, -0.245437, -0.480483, -0.716937, -0.955149, -1.19507, -1.42866, -1.64053, -1.81505, -1.94159, -2.03128, -2.1002, -2.15571, -2.20013, -2.23573, -2.26465, -2.28807, -2.30662, -2.32097, -2.33208, -2.34104, -2.3489},
          {0.649012, 0.412617, 0.176128, -0.0605492, -0.297509, -0.534846, -0.772654, -1.01067, -1.24149, -1.45118, -1.62553, -1.75504, -1.85027, -1.92637, -1.99007, -2.04323, -2.08765, -2.12505, -2.15644, -2.18242, -2.20365, -2.22122, -2.23633, -2.25021},
          {0.879773, 0.637221, 0.395131, 0.153963, -0.0858238, -0.323769, -0.559412, -0.792031, -1.01569, -1.21964, -1.39298, -1.52837, -1.63393, -1.72129, -1.7959, -1.85964, -1.91432, -1.96169, -2.00267, -2.03773, -2.06744, -2.09297, -2.1157, -2.13701},
          {1.15483, 0.911089, 0.667948, 0.426006, 0.185861, -0.0518871, -0.286641, -0.517594, -0.739778, -0.944407, -1.12255, -1.26804, -1.38657, -1.48666, -1.57288, -1.64753, -1.71289, -1.77108, -1.82289, -1.86835, -1.90765, -1.94207, -1.97322, -2.00271},
          {1.48126, 1.24782, 1.01407, 0.77971, 0.544444, 0.307966, 0.069974, -0.169551, -0.404978, -0.625484, -0.820053, -0.980742, -1.11282, -1.22477, -1.32162, -1.40636, -1.48201, -1.5513, -1.61469, -1.67134, -1.72073, -1.76418, -1.80366, -1.84112},
          {1.85499, 1.62938, 1.40263, 1.1736, 0.941171, 0.704198, 0.46155, 0.212436, -0.0370845, -0.274663, -0.487716, -0.666868, -0.816542, -0.944587, -1.05602, -1.15421, -1.24253, -1.32401, -1.39885, -1.46561, -1.52331, -1.57353, -1.61875, -1.66143},
          {2.26595, 2.02395, 1.78184, 1.53951, 1.29686, 1.05378, 0.810161, 0.566047, 0.324568, 0.0916851, -0.126538, -0.324991, -0.502678, -0.659844, -0.798134, -0.920003, -1.02792, -1.12415, -1.20914, -1.28229, -1.34343, -1.39491, -1.43993, -1.48167},
          {2.67499, 2.43049, 2.18587, 1.94099, 1.69572, 1.44994, 1.20351, 0.956329, 0.708481, 0.460258, 0.211959, -0.0333132, -0.260398, -0.452497, -0.609, -0.738689, -0.850465, -0.95252, -1.04622, -1.12908, -1.19931, -1.25902, -1.31171, -1.36084},
          {2.77556, 2.53467, 2.29352, 2.05188, 1.80948, 1.56607, 1.32141, 1.07533, 0.829515, 0.587335, 0.352224, 0.127939, -0.080366, -0.267582, -0.43283, -0.57768, -0.703738, -0.812667, -0.906719, -0.988476, -1.0605, -1.12525, -1.18512, -1.24253},
          {2.92944, 2.69029, 2.45114, 2.21195, 1.97273, 1.73345, 1.49411, 1.25471, 1.01559, 0.777432, 0.540904, 0.308644, 0.0917128, -0.0977461, -0.259486, -0.400166, -0.526527, -0.644654, -0.754232, -0.851348, -0.933036, -1.00174, -1.06176, -1.11738},
          {3.16241, 2.92093, 2.67968, 2.43889, 2.19878, 1.95959, 1.72154, 1.48483, 1.24905, 1.01326, 0.776493, 0.540052, 0.315076, 0.114247, -0.0608069, -0.214871, -0.352811, -0.479028, -0.593452, -0.693498, -0.777393, -0.847969, -0.90963, -0.96678},
      //    {3.46329, 3.22295, 2.98248, 2.74177, 2.50068, 2.2591, 2.01691, 1.77397, 1.53019, 1.28547, 1.03972, 0.794661, 0.559706, 0.345562, 0.15488, -0.014363, -0.164249, -0.29681, -0.413622, -0.516001, -0.605499, -0.684982, -0.757761, -0.827148},
      //    {3.82012, 3.58533, 3.34951, 3.11164, 2.87069, 2.62565, 2.37548, 2.11928, 1.85812, 1.59495, 1.33279, 1.07537, 0.829483, 0.602387, 0.397352, 0.215353, 0.0573356, -0.0762328, -0.189519, -0.289296, -0.381735, -0.469547, -0.554262, -0.63741},          
      //    {4.22001, 3.9812, 3.74139, 3.49961, 3.25488, 3.0062, 2.75259, 2.49319, 2.22958, 1.96561, 1.70518, 1.45138, 1.20379, 0.961575, 0.729853, 0.517157, 0.33207, 0.181896, 0.0615873, -0.0408558, -0.136201, -0.22805, -0.317559, -0.405885},
     //     {4.60906, 4.38116, 4.15169, 3.91908, 3.68176, 3.43817, 3.18673, 2.92612, 2.65952, 2.3943, 2.13798, 1.89497, 1.6562, 1.41052, 1.16192, 0.92315, 0.707102, 0.525339, 0.376692, 0.252831, 0.145883, 0.0508938, -0.0360909, -0.119026}}};
         {3.46329, 3.22295, 2.98248, 2.74177, 2.50068, 2.2591, 2.01691, 1.77397, 1.53019, 1.28547, 1.03972, 0.794661, 0.559706, 0.345562, 0.15488, -0.014363, -0.164249, -0.29681, -0.413622, -0.516001, -0.605499, -0.684982, -0.757761, -0.777148},
       {3.82012, 3.58533, 3.34951, 3.11164, 2.87069, 2.62565, 2.37548, 2.11928, 1.85812, 1.59495, 1.33279, 1.07537, 0.829483, 0.602387, 0.397352, 0.215353, 0.0573356, -0.0762328, -0.189519, -0.289296, -0.381735, -0.469547, -0.554262, -0.60741},          
          {4.22001, 3.9812, 3.74139, 3.49961, 3.25488, 3.0062, 2.75259, 2.49319, 2.22958, 1.96561, 1.70518, 1.45138, 1.20379, 0.961575, 0.729853, 0.517157, 0.33207, 0.181896, 0.0615873, -0.0408558, -0.136201, -0.22805, -0.317559, -0.375885},
          {4.60906, 4.38116, 4.15169, 3.91908, 3.68176, 3.43817, 3.18673, 2.92612, 2.65952, 2.3943, 2.13798, 1.89497, 1.6562, 1.41052, 1.16192, 0.92315, 0.707102, 0.525339, 0.376692, 0.252831, 0.145883, 0.0508938, -0.0360909, -0.089026}}};


    size_t isb1;
    size_t isb2;
    size_t i;
    double ang;
    double x1;
    double x2;
    double x;

    isb1 = static_cast<int>(sb) - 4;
    if (static_cast<int>(sb) - 4 < 0)
        isb1 = 0;
    if (isb1 > 22)
        isb1 = 22;
    isb2 = isb1 + 1;

    ang = log10(angle);

    if (ang <= angx[0])
    {
        x1 = (ang - angx[0]) / (angx[1] - angx[0]) * (sbx[isb1][1] - sbx[isb1][0]) + sbx[isb1][0];
        x2 = (ang - angx[0]) / (angx[1] - angx[0]) * (sbx[isb2][1] - sbx[isb2][0]) + sbx[isb2][0];
    }
    else if (ang >= angx[23])
    {
        x1 = (ang - angx[22]) / (angx[23] - angx[22]) * (sbx[isb1][23] - sbx[isb1][22]) + sbx[isb1][22];
        x2 = (ang - angx[22]) / (angx[23] - angx[22]) * (sbx[isb2][23] - sbx[isb2][22]) + sbx[isb2][22];
    }
    else
    {
        for (i = 0; i <= 22; i++)
        {
            if ((angx[i] < ang) && (angx[i + 1] >= ang))
                break;
        }
        x1 = (ang - angx[i]) / (angx[i + 1] - angx[i]) * (sbx[isb1][i + 1] - sbx[isb1][i]) + sbx[isb1][i];
        x2 = (ang - angx[i]) / (angx[i + 1] - angx[i]) * (sbx[isb2][i + 1] - sbx[isb2][i]) + sbx[isb2][i];
    }

    if (sb < 4.0)
    {
        x = x1;
    }
    else if (sb >= 27.0)
    {
        x = (sb - 27.0) * (x2 - x1) + x2;
    }
    else
    {
        x = (sb - static_cast<double>(static_cast<int>(sb))) * (x2 - x1) + x1;
    }

    if (x > 37.0)
        x = 37.0;
    if (x < -37.0)
        x = -37.0;

    return pow(10.0, x);
}

double astrocalc::calc_thrconls(const double angle, const double sb) const noexcept
{
    // As per Clark's book
    static constexpr double angx[7] = {-0.2255, 0.5563, 0.9859, 1.2601, 1.7419, 2.0828, 2.5563};

    static constexpr double sbx[24][7] =
        {{-0.376911, -1.806444, -2.336825, -2.460093, -2.546934, -2.560968, -2.565971},
         {-0.331451, -1.774717, -2.333688, -2.460781, -2.546521, -2.560729, -2.565756},
         {-0.268183, -1.734482, -2.331011, -2.460521, -2.546681, -2.560819, -2.565837},
         {-0.198177, -1.685101, -2.313951, -2.457228, -2.548051, -2.561529, -2.566460},
         {-0.123815, -1.625192, -2.279119, -2.446245, -2.546321, -2.559690, -2.564570},
         {-0.042444, -1.552937, -2.229734, -2.421422, -2.534318, -2.550130, -2.555202},
         {+0.049842, -1.465538, -2.165945, -2.376266, -2.504717, -2.526875, -2.533309},
         {+0.159558, -1.358054, -2.081004, -2.303572, -2.449928, -2.482261, -2.493710},
         {+0.293354, -1.225640, -1.967364, -2.196521, -2.363052, -2.409170, -2.431807},
         {+0.455739, -1.067267, -1.818647, -2.053094, -2.244490, -2.308296, -2.349087},
         {+0.649990, -0.884068, -1.629165, -1.874059, -2.098851, -2.184839, -2.250534},
         {+0.880777, -0.668706, -1.396670, -1.661059, -1.928370, -2.041067, -2.137519},
         {+1.155840, -0.395243, -1.126426, -1.417576, -1.729969, -1.872722, -2.003406},
         {+1.482228, -0.041944, -0.824314, -1.147481, -1.502126, -1.676828, -1.842016},
         {+1.855923, +0.345791, -0.492425, -0.856105, -1.266128, -1.472054, -1.662449},
         {+2.266949, +0.696000, -0.131546, -0.550963, -1.056193, -1.289210, -1.482659},
         {+2.676000, +1.088000, +0.206000, -0.321000, -0.880000, -1.137000, -1.362000},
         {+2.776553, +1.206474, +0.346695, -0.137740, -0.736142, -0.996400, -1.243900},
         {+2.930431, +1.382138, +0.535251, +0.032813, -0.560534, -0.860600, -1.118700},
         {+3.163406, +1.610655, +0.770789, +0.253144, -0.389500, -0.703000, -0.968133},
         {+3.464285, +1.903381, +1.033812, +0.494252, -0.203342, -0.525901, -0.828800},
         {+3.821092, +2.256431, +1.326535, +0.760529, +0.017200, -0.299200, -0.639400},
         {+4.221000, +2.632000, +1.699000, +1.132000, +0.286000, -0.051000, -0.408000},
         {+4.610000, +3.066000, +2.132000, +1.585000, +0.652000, +0.241000, -0.121000}};

    int isb1;
    int isb2;
    int i;
    double ang;
    double x1;
    double x2;
    double x;

    isb1 = static_cast<int>(sb) - 4;
    if (isb1 < 0)
        isb1 = 0;
    if (isb1 > 22)
        isb1 = 22;
    isb2 = isb1 + 1;

    ang = log10(angle);

    if (ang <= angx[0])
    {
        //   x1 = sbx[isb1][0];
        //   x2 = sbx[isb2][0];

        x1 = (ang - angx[0]) / (angx[1] - angx[0]) * (sbx[isb1][1] - sbx[isb1][0]) + sbx[isb1][0];
        x2 = (ang - angx[0]) / (angx[1] - angx[0]) * (sbx[isb2][1] - sbx[isb2][0]) + sbx[isb2][0];
    }
    else if (ang >= angx[6])
    {
        x1 = (ang - angx[5]) / (angx[6] - angx[5]) * (sbx[isb1][6] - sbx[isb1][5]) + sbx[isb1][5];
        x2 = (ang - angx[5]) / (angx[6] - angx[5]) * (sbx[isb2][6] - sbx[isb2][5]) + sbx[isb2][5];
    }
    else
    {
        for (i = 0; i <= 5; i++)
        {
            if ((angx[i] < ang) && (angx[i + 1] >= ang))
                break;
        }
        x1 = (ang - angx[i]) / (angx[i + 1] - angx[i]) * (sbx[isb1][i + 1] - sbx[isb1][i]) + sbx[isb1][i];
        x2 = (ang - angx[i]) / (angx[i + 1] - angx[i]) * (sbx[isb2][i + 1] - sbx[isb2][i]) + sbx[isb2][i];
    }

    if (sb < 4.0)
    {
        x = x1;
    }
    else if (sb >= 27.0)
    {
        x = (sb - 27.0) * (x2 - x1) + x2;
    }
    else
    {
        x = (sb - static_cast<double>(static_cast<int>(sb))) * (x2 - x1) + x1;
    }

    if (x > 37.0)
        x = 37.0;
    if (x < -37.0)
        x = -37.0;

    return pow(10.0, x);
}

std::pair<double, double> astrocalc::calc_dso_contrast_in_scope(const double magnification, const int scopetype, const double scopeaperture,
                                                                const double scopeobstruct, const double scopereflect, const double etrans,
                                                                const double dirt, const double eyepupilsize, const double nelm, const double dsovmag,
                                                                const double minoraxis, const double majoraxis) const noexcept
{
    // WIP to be refined.

    //double m_zenithangle = 45 / RADTODEG;
    // double extcof = 0.3;
    double fl = 0.0;
    double fd = 0.0;
    double metrans = etrans;

    if (REFLECTOR == scopetype) // reflectors
    {
        fl = pow(scopereflect, 2);        // reflectivity effciency due to primary and secondary.
        fd = 1.0 - pow(scopeobstruct, 2); // net effect due to obstruction.
    }

    if (REFRACTOR == scopetype) // refractors
    {
        fd = 1.0;
        fl = pow(scopereflect, 4);
    }

    if (SCTMAK == scopetype)
    {
        fd = 1.0 - pow(scopeobstruct, 2);
        fl = pow(SCOPETRANS / 100.0 * scopereflect, 2); // SCT Mak modify tcalc app to call this correctly, make little diff though.
    }

    if (REFLECTOR != scopetype)
        metrans = metrans * 0.97; // we'll assume the added diagonal has 97% reflectivity

    metrans *= 1 - dirt;           // effect of dirt on transmission
    double ft = fl * fd * metrans; // net transmission

    double bg_brightness = calc_nelm_brightness_threshold_method(nelm); // sky background

    // double zenfactor = 1.0 + 0.5 * m_zenithangle * m_zenithangle;
    //  double extfactor = pow(10, 0.4 * 1.2 * extcof / cos(m_zenithangle));
    //  double scopefactor = -5.0 * log10((sqrt(ft / (zenfactor * extfactor)) / eyepupilsize) * (scopeaperture / magnification)); // sky in scope contribution
    double scopefactor = -5.0 * log10((sqrt(ft) / eyepupilsize) * (scopeaperture / magnification));
    double obscontrast = -log10(calc_thrconls(magnification * minoraxis, bg_brightness + scopefactor));
    double objbrightness = calc_dso_mag_to_brightness(dsovmag, minoraxis, majoraxis); // object brightness from vmag
    double obscontrastthreshold = obscontrast + calc_contrast_index(bg_brightness, objbrightness);
    return std::make_pair(obscontrastthreshold, -obscontrast); // threshold
}