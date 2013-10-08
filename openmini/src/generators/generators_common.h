/// @filename generators_common.h
/// @brief OpenMini common stuff for all generators - declarations
/// @author gm
/// @copyright gm 2013
///
/// This file is part of OpenMini
///
/// OpenMini is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// OpenMini is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with OpenMini.  If not, see <http://www.gnu.org/licenses/>.

#ifndef OPENMINI_SRC_GENERATORS_GENERATORS_COMMON_H_
#define OPENMINI_SRC_GENERATORS_GENERATORS_COMMON_H_

// std::min, std::max
#include <algorithm>

#include "openmini/src/common.h"
#include "openmini/src/generators/generator_interface.h"

namespace openmini {
namespace generators {

/// @brief Random generator seed
// It is prime, and mutually prime with the LCG multiplicator
static int rng_seed = 317;

/// @brief Init random seed
// Implementation detail:
// Defined "static inline" to be accessible from everywhere
// without having the linker complaining
static inline void InitSeed(int seed) {
  rng_seed = seed;
}

/// @brief Generates normalized random floats from an uniform distribution
class GeneratorNormFloatRand {
 public:
  /// @brief Actual generation functor
  float operator()(void) const {
    rng_seed *= 16807;
    return static_cast<float>(rng_seed) * 4.6566129e-010f;
  }
};

/// @brief Basic sawtooth signal generator
/// Generates a lot of aliasing, not to be used straight to audio
class PhaseAccumulator : public Generator_Interface {
 public:
  PhaseAccumulator();
  virtual float operator()(void);
  virtual void SetPhase(const float phase);
  virtual void SetFrequency(const float frequency);

 protected:
  virtual void ProcessParameters(void);

 private:
  float increment_;  ///< Value to be added at the phase at each iteration
  float current_;  ///< Current phase value
  float frequency_;  ///< Frequency to be set, allows asynchronous update
  bool update_;  ///< True if internal values have to be changed
};

/// @brief Basic differentiator
/// implementing a simple 1st-order differentiator, unitary gain
class Differentiator {
 public:
  Differentiator();
  float operator()(const float sample);

 private:
  float last_;  ///< Last synthesized sample value
};

/// @brief One pole signal filter class
class OnePoleFilter {
 public:
  OnePoleFilter(const double b0_, const double b1_);
  void init(void);
  float operator()(const float input);

 private:
  double b0_;  ///< Gain value
  double b1_;  ///< Pole value
  float last_;  ///< Last processed sample value
};

// Miscellaneous

/// @brief Table for Error function values
static const float kErfTable[] = {
  -0.999977909503f,
  -0.999977078238f,
  -0.99997621726f,
  -0.999975325566f,
  -0.999974402126f,
  -0.999973445873f,
  -0.999972455712f,
  -0.999971430509f,
  -0.999970369097f,
  -0.999969270275f,
  -0.9999681328f,
  -0.999966955395f,
  -0.999965736741f,
  -0.999964475481f,
  -0.999963170214f,
  -0.999961819498f,
  -0.999960421846f,
  -0.999958975728f,
  -0.999957479565f,
  -0.999955931733f,
  -0.999954330556f,
  -0.99995267431f,
  -0.999950961219f,
  -0.999949189455f,
  -0.999947357133f,
  -0.999945462314f,
  -0.999943503001f,
  -0.999941477139f,
  -0.999939382611f,
  -0.999937217239f,
  -0.99993497878f,
  -0.999932664928f,
  -0.999930273306f,
  -0.999927801472f,
  -0.99992524691f,
  -0.999922607035f,
  -0.999919879183f,
  -0.999917060618f,
  -0.999914148523f,
  -0.999911140001f,
  -0.999908032073f,
  -0.999904821676f,
  -0.999901505659f,
  -0.999898080783f,
  -0.999894543718f,
  -0.99989089104f,
  -0.99988711923f,
  -0.99988322467f,
  -0.999879203642f,
  -0.999875052324f,
  -0.999870766791f,
  -0.999866343007f,
  -0.999861776826f,
  -0.999857063988f,
  -0.99985220012f,
  -0.999847180724f,
  -0.999842001186f,
  -0.999836656762f,
  -0.999831142584f,
  -0.999825453651f,
  -0.999819584829f,
  -0.999813530845f,
  -0.999807286289f,
  -0.999800845602f,
  -0.999794203083f,
  -0.999787352879f,
  -0.999780288981f,
  -0.999773005224f,
  -0.999765495282f,
  -0.999757752665f,
  -0.999749770712f,
  -0.999741542591f,
  -0.999733061294f,
  -0.999724319633f,
  -0.999715310233f,
  -0.999706025534f,
  -0.999696457782f,
  -0.999686599026f,
  -0.999676441114f,
  -0.999665975688f,
  -0.99965519418f,
  -0.999644087809f,
  -0.999632647572f,
  -0.999620864245f,
  -0.999608728372f,
  -0.999596230266f,
  -0.99958336f,
  -0.999570107405f,
  -0.999556462061f,
  -0.999542413296f,
  -0.999527950178f,
  -0.99951306151f,
  -0.999497735825f,
  -0.999481961382f,
  -0.999465726157f,
  -0.99944901784f,
  -0.999431823828f,
  -0.99941413122f,
  -0.999395926809f,
  -0.99937719708f,
  -0.999357928198f,
  -0.999338106009f,
  -0.999317716025f,
  -0.999296743427f,
  -0.99927517305f,
  -0.999252989383f,
  -0.999230176558f,
  -0.999206718346f,
  -0.999182598147f,
  -0.999157798987f,
  -0.999132303506f,
  -0.999106093959f,
  -0.999079152197f,
  -0.999051459672f,
  -0.999022997419f,
  -0.998993746058f,
  -0.998963685778f,
  -0.998932796334f,
  -0.998901057039f,
  -0.998868446756f,
  -0.998834943887f,
  -0.998800526368f,
  -0.998765171663f,
  -0.99872885675f,
  -0.998691558117f,
  -0.998653251751f,
  -0.998613913133f,
  -0.998573517224f,
  -0.998532038463f,
  -0.998489450752f,
  -0.998445727452f,
  -0.998400841371f,
  -0.998354764756f,
  -0.998307469283f,
  -0.99825892605f,
  -0.998209105566f,
  -0.998157977742f,
  -0.998105511882f,
  -0.998051676673f,
  -0.997996440176f,
  -0.997939769815f,
  -0.997881632368f,
  -0.99782199396f,
  -0.997760820047f,
  -0.997698075412f,
  -0.997633724153f,
  -0.99756772967f,
  -0.99750005466f,
  -0.997430661103f,
  -0.997359510254f,
  -0.997286562629f,
  -0.997211778002f,
  -0.997135115385f,
  -0.997056533025f,
  -0.996975988391f,
  -0.996893438162f,
  -0.996808838218f,
  -0.996722143631f,
  -0.99663330865f,
  -0.996542286693f,
  -0.996449030336f,
  -0.996353491304f,
  -0.996255620455f,
  -0.996155367774f,
  -0.996052682361f,
  -0.995947512419f,
  -0.995839805244f,
  -0.995729507213f,
  -0.995616563775f,
  -0.995500919439f,
  -0.995382517761f,
  -0.995261301339f,
  -0.995137211794f,
  -0.995010189766f,
  -0.9948801749f,
  -0.994747105834f,
  -0.994610920193f,
  -0.994471554572f,
  -0.994328944528f,
  -0.994183024573f,
  -0.994033728155f,
  -0.993880987655f,
  -0.993724734373f,
  -0.993564898517f,
  -0.993401409194f,
  -0.993234194399f,
  -0.993063181006f,
  -0.992888294753f,
  -0.992709460238f,
  -0.992526600906f,
  -0.992339639037f,
  -0.992148495739f,
  -0.99195309094f,
  -0.991753343372f,
  -0.991549170566f,
  -0.991340488844f,
  -0.991127213303f,
  -0.990909257815f,
  -0.990686535008f,
  -0.990458956266f,
  -0.990226431715f,
  -0.989988870215f,
  -0.989746179353f,
  -0.989498265433f,
  -0.989245033472f,
  -0.988986387186f,
  -0.988722228985f,
  -0.988452459969f,
  -0.988176979914f,
  -0.98789568727f,
  -0.987608479153f,
  -0.987315251336f,
  -0.987015898248f,
  -0.98671031296f,
  -0.986398387186f,
  -0.986080011274f,
  -0.9857550742f,
  -0.985423463567f,
  -0.985085065594f,
  -0.984739765117f,
  -0.984387445582f,
  -0.984027989038f,
  -0.983661276142f,
  -0.983287186147f,
  -0.982905596903f,
  -0.982516384852f,
  -0.98211942503f,
  -0.98171459106f,
  -0.981301755152f,
  -0.980880788104f,
  -0.980451559297f,
  -0.980013936697f,
  -0.979567786856f,
  -0.979112974907f,
  -0.978649364571f,
  -0.978176818154f,
  -0.977695196549f,
  -0.977204359238f,
  -0.976704164294f,
  -0.976194468386f,
  -0.975675126779f,
  -0.975145993337f,
  -0.974606920531f,
  -0.97405775944f,
  -0.973498359759f,
  -0.9729285698f,
  -0.972348236502f,
  -0.971757205435f,
  -0.971155320809f,
  -0.970542425478f,
  -0.969918360954f,
  -0.969282967407f,
  -0.968636083681f,
  -0.9679775473f,
  -0.967307194481f,
  -0.966624860139f,
  -0.965930377906f,
  -0.965223580134f,
  -0.964504297915f,
  -0.963772361089f,
  -0.963027598258f,
  -0.962269836804f,
  -0.961498902895f,
  -0.960714621511f,
  -0.95991681645f,
  -0.959105310352f,
  -0.958279924708f,
  -0.957440479887f,
  -0.956586795145f,
  -0.95571868865f,
  -0.9548359775f,
  -0.95393847774f,
  -0.953026004387f,
  -0.952098371447f,
  -0.951155391941f,
  -0.950196877927f,
  -0.949222640517f,
  -0.948232489911f,
  -0.947226235413f,
  -0.946203685461f,
  -0.94516464765f,
  -0.944108928759f,
  -0.94303633478f,
  -0.941946670945f,
  -0.94083974175f,
  -0.939715350993f,
  -0.938573301793f,
  -0.93741339663f,
  -0.936235437369f,
  -0.935039225297f,
  -0.933824561151f,
  -0.932591245153f,
  -0.931339077043f,
  -0.930067856115f,
  -0.928777381249f,
  -0.92746745095f,
  -0.926137863379f,
  -0.924788416397f,
  -0.923418907596f,
  -0.922029134341f,
  -0.920618893807f,
  -0.919187983018f,
  -0.917736198891f,
  -0.916263338268f,
  -0.914769197969f,
  -0.913253574823f,
  -0.911716265716f,
  -0.910157067634f,
  -0.908575777705f,
  -0.906972193244f,
  -0.905346111796f,
  -0.903697331184f,
  -0.902025649555f,
  -0.900330865423f,
  -0.89861277772f,
  -0.896871185841f,
  -0.895105889691f,
  -0.893316689739f,
  -0.89150338706f,
  -0.889665783389f,
  -0.887803681172f,
  -0.885916883611f,
  -0.88400519472f,
  -0.882068419377f,
  -0.880106363371f,
  -0.878118833457f,
  -0.876105637412f,
  -0.874066584081f,
  -0.872001483437f,
  -0.869910146632f,
  -0.867792386051f,
  -0.865648015367f,
  -0.863476849597f,
  -0.861278705155f,
  -0.859053399911f,
  -0.856800753243f,
  -0.854520586096f,
  -0.852212721035f,
  -0.849876982307f,
  -0.847513195893f,
  -0.845121189566f,
  -0.84270079295f,
  -0.840251837576f,
  -0.837774156939f,
  -0.835267586559f,
  -0.832731964034f,
  -0.8301671291f,
  -0.827572923692f,
  -0.824949191995f,
  -0.82229578051f,
  -0.819612538107f,
  -0.816899316083f,
  -0.814155968226f,
  -0.811382350863f,
  -0.808578322927f,
  -0.805743746012f,
  -0.802878484427f,
  -0.799982405261f,
  -0.797055378434f,
  -0.794097276757f,
  -0.79110797599f,
  -0.788087354897f,
  -0.785035295305f,
  -0.781951682158f,
  -0.778836403574f,
  -0.775689350905f,
  -0.772510418784f,
  -0.769299505189f,
  -0.766056511492f,
  -0.762781342517f,
  -0.759473906592f,
  -0.756134115603f,
  -0.752761885048f,
  -0.749357134091f,
  -0.74591978561f,
  -0.742449766252f,
  -0.738947006484f,
  -0.735411440644f,
  -0.731843006987f,
  -0.72824164774f,
  -0.724607309147f,
  -0.720939941518f,
  -0.717239499279f,
  -0.713505941012f,
  -0.70973922951f,
  -0.705939331813f,
  -0.702106219261f,
  -0.69823986753f,
  -0.694340256681f,
  -0.690407371198f,
  -0.68644120003f,
  -0.682441736634f,
  -0.678408979009f,
  -0.67434292974f,
  -0.670243596031f,
  -0.666110989746f,
  -0.661945127438f,
  -0.657746030391f,
  -0.653513724648f,
  -0.649248241046f,
  -0.644949615246f,
  -0.640617887765f,
  -0.636253104002f,
  -0.631855314271f,
  -0.627424573823f,
  -0.622960942874f,
  -0.618464486629f,
  -0.613935275307f,
  -0.609373384161f,
  -0.604778893498f,
  -0.600151888703f,
  -0.595492460254f,
  -0.59080070374f,
  -0.586076719874f,
  -0.581320614514f,
  -0.576532498672f,
  -0.571712488524f,
  -0.566860705425f,
  -0.561977275914f,
  -0.557062331727f,
  -0.552116009798f,
  -0.547138452267f,
  -0.542129806482f,
  -0.537090225004f,
  -0.532019865604f,
  -0.526918891266f,
  -0.521787470182f,
  -0.51662577575f,
  -0.511433986569f,
  -0.506212286431f,
  -0.500960864316f,
  -0.49567991438f,
  -0.490369635943f,
  -0.485030233483f,
  -0.479661916613f,
  -0.474264900073f,
  -0.468839403711f,
  -0.463385652461f,
  -0.457903876328f,
  -0.452394310366f,
  -0.446857194652f,
  -0.441292774265f,
  -0.435701299258f,
  -0.430083024631f,
  -0.424438210303f,
  -0.418767121081f,
  -0.413070026628f,
  -0.40734720143f,
  -0.401598924762f,
  -0.395825480649f,
  -0.390027157831f,
  -0.384204249722f,
  -0.378357054372f,
  -0.37248587442f,
  -0.366591017056f,
  -0.360672793971f,
  -0.354731521313f,
  -0.348767519641f,
  -0.342781113872f,
  -0.336772633232f,
  -0.330742411204f,
  -0.324690785475f,
  -0.31861809788f,
  -0.312524694348f,
  -0.306410924843f,
  -0.300277143303f,
  -0.294123707587f,
  -0.287950979406f,
  -0.281759324264f,
  -0.275549111395f,
  -0.269320713696f,
  -0.263074507663f,
  -0.256810873321f,
  -0.250530194157f,
  -0.24423285705f,
  -0.2379192522f,
  -0.231589773057f,
  -0.225244816245f,
  -0.218884781494f,
  -0.212510071557f,
  -0.20612109214f,
  -0.199718251823f,
  -0.193301961979f,
  -0.1868726367f,
  -0.180430692715f,
  -0.173976549308f,
  -0.167510628236f,
  -0.161033353649f,
  -0.154545152006f,
  -0.148046451987f,
  -0.141537684416f,
  -0.135019282167f,
  -0.128491680083f,
  -0.121955314886f,
  -0.115410625094f,
  -0.108858050925f,
  -0.102298034217f,
  -0.0957310183315f,
  -0.0891574480689f,
  -0.0825777695745f,
  -0.0759924302492f,
  -0.0694018786581f,
  -0.0628065644389f,
  -0.0562069382096f,
  -0.0496034514764f,
  -0.0429965565409f,
  -0.0363867064072f,
  -0.0297743546889f,
  -0.0231599555153f,
  -0.0165439634381f,
  -0.00992683333788f,
  -0.00330902032983f,
  0.00330902032983f,
  0.00992683333788f,
  0.0165439634381f,
  0.0231599555153f,
  0.0297743546889f,
  0.0363867064072f,
  0.0429965565409f,
  0.0496034514764f,
  0.0562069382096f,
  0.0628065644389f,
  0.0694018786581f,
  0.0759924302492f,
  0.0825777695745f,
  0.0891574480689f,
  0.0957310183315f,
  0.102298034217f,
  0.108858050925f,
  0.115410625094f,
  0.121955314886f,
  0.128491680083f,
  0.135019282167f,
  0.141537684416f,
  0.148046451987f,
  0.154545152006f,
  0.161033353649f,
  0.167510628236f,
  0.173976549308f,
  0.180430692715f,
  0.1868726367f,
  0.193301961979f,
  0.199718251823f,
  0.20612109214f,
  0.212510071557f,
  0.218884781494f,
  0.225244816245f,
  0.231589773057f,
  0.2379192522f,
  0.24423285705f,
  0.250530194157f,
  0.256810873321f,
  0.263074507663f,
  0.269320713696f,
  0.275549111395f,
  0.281759324264f,
  0.287950979406f,
  0.294123707587f,
  0.300277143303f,
  0.306410924843f,
  0.312524694348f,
  0.31861809788f,
  0.324690785475f,
  0.330742411204f,
  0.336772633232f,
  0.342781113872f,
  0.348767519641f,
  0.354731521313f,
  0.360672793971f,
  0.366591017056f,
  0.37248587442f,
  0.378357054372f,
  0.384204249722f,
  0.390027157831f,
  0.395825480649f,
  0.401598924762f,
  0.40734720143f,
  0.413070026628f,
  0.418767121081f,
  0.424438210303f,
  0.430083024631f,
  0.435701299258f,
  0.441292774265f,
  0.446857194652f,
  0.452394310366f,
  0.457903876328f,
  0.463385652461f,
  0.468839403711f,
  0.474264900073f,
  0.479661916613f,
  0.485030233483f,
  0.490369635943f,
  0.49567991438f,
  0.500960864316f,
  0.506212286431f,
  0.511433986569f,
  0.51662577575f,
  0.521787470182f,
  0.526918891266f,
  0.532019865604f,
  0.537090225004f,
  0.542129806482f,
  0.547138452267f,
  0.552116009798f,
  0.557062331727f,
  0.561977275914f,
  0.566860705425f,
  0.571712488524f,
  0.576532498672f,
  0.581320614514f,
  0.586076719874f,
  0.59080070374f,
  0.595492460254f,
  0.600151888703f,
  0.604778893498f,
  0.609373384161f,
  0.613935275307f,
  0.618464486629f,
  0.622960942874f,
  0.627424573823f,
  0.631855314271f,
  0.636253104002f,
  0.640617887765f,
  0.644949615246f,
  0.649248241046f,
  0.653513724648f,
  0.657746030391f,
  0.661945127438f,
  0.666110989746f,
  0.670243596031f,
  0.67434292974f,
  0.678408979009f,
  0.682441736634f,
  0.68644120003f,
  0.690407371198f,
  0.694340256681f,
  0.69823986753f,
  0.702106219261f,
  0.705939331813f,
  0.70973922951f,
  0.713505941012f,
  0.717239499279f,
  0.720939941518f,
  0.724607309147f,
  0.72824164774f,
  0.731843006987f,
  0.735411440644f,
  0.738947006484f,
  0.742449766252f,
  0.74591978561f,
  0.749357134091f,
  0.752761885048f,
  0.756134115603f,
  0.759473906592f,
  0.762781342517f,
  0.766056511492f,
  0.769299505189f,
  0.772510418784f,
  0.775689350905f,
  0.778836403574f,
  0.781951682158f,
  0.785035295305f,
  0.788087354897f,
  0.79110797599f,
  0.794097276757f,
  0.797055378434f,
  0.799982405261f,
  0.802878484427f,
  0.805743746012f,
  0.808578322927f,
  0.811382350863f,
  0.814155968226f,
  0.816899316083f,
  0.819612538107f,
  0.82229578051f,
  0.824949191995f,
  0.827572923692f,
  0.8301671291f,
  0.832731964034f,
  0.835267586559f,
  0.837774156939f,
  0.840251837576f,
  0.84270079295f,
  0.845121189566f,
  0.847513195893f,
  0.849876982307f,
  0.852212721035f,
  0.854520586096f,
  0.856800753243f,
  0.859053399911f,
  0.861278705155f,
  0.863476849597f,
  0.865648015367f,
  0.867792386051f,
  0.869910146632f,
  0.872001483437f,
  0.874066584081f,
  0.876105637412f,
  0.878118833457f,
  0.880106363371f,
  0.882068419377f,
  0.88400519472f,
  0.885916883611f,
  0.887803681172f,
  0.889665783389f,
  0.89150338706f,
  0.893316689739f,
  0.895105889691f,
  0.896871185841f,
  0.89861277772f,
  0.900330865423f,
  0.902025649555f,
  0.903697331184f,
  0.905346111796f,
  0.906972193244f,
  0.908575777705f,
  0.910157067634f,
  0.911716265716f,
  0.913253574823f,
  0.914769197969f,
  0.916263338268f,
  0.917736198891f,
  0.919187983018f,
  0.920618893807f,
  0.922029134341f,
  0.923418907596f,
  0.924788416397f,
  0.926137863379f,
  0.92746745095f,
  0.928777381249f,
  0.930067856115f,
  0.931339077043f,
  0.932591245153f,
  0.933824561151f,
  0.935039225297f,
  0.936235437369f,
  0.93741339663f,
  0.938573301793f,
  0.939715350993f,
  0.94083974175f,
  0.941946670945f,
  0.94303633478f,
  0.944108928759f,
  0.94516464765f,
  0.946203685461f,
  0.947226235413f,
  0.948232489911f,
  0.949222640517f,
  0.950196877927f,
  0.951155391941f,
  0.952098371447f,
  0.953026004387f,
  0.95393847774f,
  0.9548359775f,
  0.95571868865f,
  0.956586795145f,
  0.957440479887f,
  0.958279924708f,
  0.959105310352f,
  0.95991681645f,
  0.960714621511f,
  0.961498902895f,
  0.962269836804f,
  0.963027598258f,
  0.963772361089f,
  0.964504297915f,
  0.965223580134f,
  0.965930377906f,
  0.966624860139f,
  0.967307194481f,
  0.9679775473f,
  0.968636083681f,
  0.969282967407f,
  0.969918360954f,
  0.970542425478f,
  0.971155320809f,
  0.971757205435f,
  0.972348236502f,
  0.9729285698f,
  0.973498359759f,
  0.97405775944f,
  0.974606920531f,
  0.975145993337f,
  0.975675126779f,
  0.976194468386f,
  0.976704164294f,
  0.977204359238f,
  0.977695196549f,
  0.978176818154f,
  0.978649364571f,
  0.979112974907f,
  0.979567786856f,
  0.980013936697f,
  0.980451559297f,
  0.980880788104f,
  0.981301755152f,
  0.98171459106f,
  0.98211942503f,
  0.982516384852f,
  0.982905596903f,
  0.983287186147f,
  0.983661276142f,
  0.984027989038f,
  0.984387445582f,
  0.984739765117f,
  0.985085065594f,
  0.985423463567f,
  0.9857550742f,
  0.986080011274f,
  0.986398387186f,
  0.98671031296f,
  0.987015898248f,
  0.987315251336f,
  0.987608479153f,
  0.98789568727f,
  0.988176979914f,
  0.988452459969f,
  0.988722228985f,
  0.988986387186f,
  0.989245033472f,
  0.989498265433f,
  0.989746179353f,
  0.989988870215f,
  0.990226431715f,
  0.990458956266f,
  0.990686535008f,
  0.990909257815f,
  0.991127213303f,
  0.991340488844f,
  0.991549170566f,
  0.991753343372f,
  0.99195309094f,
  0.992148495739f,
  0.992339639037f,
  0.992526600906f,
  0.992709460238f,
  0.992888294753f,
  0.993063181006f,
  0.993234194399f,
  0.993401409194f,
  0.993564898517f,
  0.993724734373f,
  0.993880987655f,
  0.994033728155f,
  0.994183024573f,
  0.994328944528f,
  0.994471554572f,
  0.994610920193f,
  0.994747105834f,
  0.9948801749f,
  0.995010189766f,
  0.995137211794f,
  0.995261301339f,
  0.995382517761f,
  0.995500919439f,
  0.995616563775f,
  0.995729507213f,
  0.995839805244f,
  0.995947512419f,
  0.996052682361f,
  0.996155367774f,
  0.996255620455f,
  0.996353491304f,
  0.996449030336f,
  0.996542286693f,
  0.99663330865f,
  0.996722143631f,
  0.996808838218f,
  0.996893438162f,
  0.996975988391f,
  0.997056533025f,
  0.997135115385f,
  0.997211778002f,
  0.997286562629f,
  0.997359510254f,
  0.997430661103f,
  0.99750005466f,
  0.99756772967f,
  0.997633724153f,
  0.997698075412f,
  0.997760820047f,
  0.99782199396f,
  0.997881632368f,
  0.997939769815f,
  0.997996440176f,
  0.998051676673f,
  0.998105511882f,
  0.998157977742f,
  0.998209105566f,
  0.99825892605f,
  0.998307469283f,
  0.998354764756f,
  0.998400841371f,
  0.998445727452f,
  0.998489450752f,
  0.998532038463f,
  0.998573517224f,
  0.998613913133f,
  0.998653251751f,
  0.998691558117f,
  0.99872885675f,
  0.998765171663f,
  0.998800526368f,
  0.998834943887f,
  0.998868446756f,
  0.998901057039f,
  0.998932796334f,
  0.998963685778f,
  0.998993746058f,
  0.999022997419f,
  0.999051459672f,
  0.999079152197f,
  0.999106093959f,
  0.999132303506f,
  0.999157798987f,
  0.999182598147f,
  0.999206718346f,
  0.999230176558f,
  0.999252989383f,
  0.99927517305f,
  0.999296743427f,
  0.999317716025f,
  0.999338106009f,
  0.999357928198f,
  0.99937719708f,
  0.999395926809f,
  0.99941413122f,
  0.999431823828f,
  0.99944901784f,
  0.999465726157f,
  0.999481961382f,
  0.999497735825f,
  0.99951306151f,
  0.999527950178f,
  0.999542413296f,
  0.999556462061f,
  0.999570107405f,
  0.99958336f,
  0.999596230266f,
  0.999608728372f,
  0.999620864245f,
  0.999632647572f,
  0.999644087809f,
  0.99965519418f,
  0.999665975688f,
  0.999676441114f,
  0.999686599026f,
  0.999696457782f,
  0.999706025534f,
  0.999715310233f,
  0.999724319633f,
  0.999733061294f,
  0.999741542591f,
  0.999749770712f,
  0.999757752665f,
  0.999765495282f,
  0.999773005224f,
  0.999780288981f,
  0.999787352879f,
  0.999794203083f,
  0.999800845602f,
  0.999807286289f,
  0.999813530845f,
  0.999819584829f,
  0.999825453651f,
  0.999831142584f,
  0.999836656762f,
  0.999842001186f,
  0.999847180724f,
  0.99985220012f,
  0.999857063988f,
  0.999861776826f,
  0.999866343007f,
  0.999870766791f,
  0.999875052324f,
  0.999879203642f,
  0.99988322467f,
  0.99988711923f,
  0.99989089104f,
  0.999894543718f,
  0.999898080783f,
  0.999901505659f,
  0.999904821676f,
  0.999908032073f,
  0.999911140001f,
  0.999914148523f,
  0.999917060618f,
  0.999919879183f,
  0.999922607035f,
  0.99992524691f,
  0.999927801472f,
  0.999930273306f,
  0.999932664928f,
  0.99993497878f,
  0.999937217239f,
  0.999939382611f,
  0.999941477139f,
  0.999943503001f,
  0.999945462314f,
  0.999947357133f,
  0.999949189455f,
  0.999950961219f,
  0.99995267431f,
  0.999954330556f,
  0.999955931733f,
  0.999957479565f,
  0.999958975728f,
  0.999960421846f,
  0.999961819498f,
  0.999963170214f,
  0.999964475481f,
  0.999965736741f,
  0.999966955395f,
  0.9999681328f,
  0.999969270275f,
  0.999970369097f,
  0.999971430509f,
  0.999972455712f,
  0.999973445873f,
  0.999974402126f,
  0.999975325566f,
  0.99997621726f,
  0.999977078238f,
  0.999977909503f,
};

/// @brief Error function values actual count
static const int kErfTableSize = sizeof(kErfTable) / sizeof(float);

/// @brief Helper function: Gauss Error function
/// Note that outside [-3 ; 3] it returns respectively -1 and 1
float ErfTabulated(const float input);

/// @brief Helper function: increment the input and wraps it into [-1.0 ; 1.0[
/// @param[in]  input         Input to be wrapped - supposed not to be < 1.0
/// @param[in]  increment     Increment to add to the input
///@return the incremented output in [-1.0 ; 1.0[
float IncrementAndWrap(const float input, const float increment);

/// @brief Helper function: Clamp input into [min ; max]
template <typename TypeInput>
void Clamp(TypeInput* input, const TypeInput min, const TypeInput max) {
    *input = std::min(std::max(*input, min), max);
}

/// @brief Helper function: Sign of the input (branchless implementation)
template <typename TypeInput>
int GetSign(const TypeInput input) {
  const TypeInput zero = static_cast<TypeInput>(0);
  return (zero < input) - (input < zero);
}

/// @brief Simple linear interpolation
///
/// --------|--------------x--|-----------
///       left              right
///         |<    ratio   >|
///
/// @param[in] left     Previous value
/// @param[in] right    Next value
/// @param[out] ratio   Location of the required interpolated value
///
/// @return The interpolated value
float LinearInterpolation(
    const float left,
    const float right,
    const float ratio);

}  // namespace generators
}  // namespace openmini

#endif  // OPENMINI_SRC_GENERATORS_GENERATORS_COMMON_H_
