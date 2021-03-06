#pragma once
#include "defs.h"
#include "distance_map.h"
#include <tr1/random>

namespace thin_localizer {

  class Particle {
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
    Particle();
    Eigen::Vector3f _pose;
    double _weight;
  };

  typedef std::vector<Particle, Eigen::aligned_allocator<Particle> > ParticleVector;

  /**
     Simple localizer class that implements a markov localization algorithm on a 2D gridmap.
     The map is represented through an 8 bit grayscale image, and a resolution meters/pixel.
  */

  class Localizer {
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

    Localizer();
  
    //! call this to load the map in the localizer
    //! @param m: an 8 bit grayscale image
    //! @param resolution: size in meters of a map pixel
    //! @param occ_threshold: values of a pixel values below this are considered occupies
    //! @param free_threshold: values of a pixel above this are free space
    void setMap(const UnsignedCharImage& m,
		float resolution,
		unsigned char occ_threshold,
		unsigned char free_threshold);
	      
    //! initializes the localizer (after setting a map)
    //! @param num_particles: the higher the more powerful the filter, you pay with computation
    //! @param dmax: max distance to compute the likelihood field. 
    //!              High values result in larger convergence basin (good for global localization), 
    //!              but poorer position tracking
    //! @param min_weight: the lower, the more faster the filter converges
    //! @param min_valid_points: minimum number of good endpoints to do the update
    void init(int num_particles=2000, 
	      float dmax=1.0f, 
	      float robot_radius=0.2,
	      float min_weight=0.01,
	      int min_valid_points=30);

    //! starts the global localization
    void startGlobal();

    //! sets all particles in a position
    void setPose(const Eigen::Vector3f pose, 
		 Eigen::Vector3f standard_deviations=Eigen::Vector3f(0.2, 0.2, 0.2));
  
    //! integrates a motion
    //! param@ control: the motion
    void predict(const Eigen::Vector3f control);

    //! integrates an observation
    //! param@ observation: the <x,y> endpoints of the valid laser beams (no maxrange)
    //! returns true if the update was not performed (the robot has not moved enough)
    bool update(const Vector2fVector observation);


    //! returns the minimum translation of the robot between updates (meters)
    inline float minUpdateTranslation() const {return _min_update_translation;}

    //! sets the minimum translation of the robot between updates (meters)
    inline void setMinUpdateTranslation(float v) { _min_update_translation=v;}

    //! returns the minimum rotation of the robot between updates (radians)
    inline float minUpdateRotation() const {return _min_update_rotation;}

    //! sets the minimum rotation of the robot between updates (radians)
    inline void setMinUpdateRotation(float v) { _min_update_rotation=v;}


    //! paints the state in a rgb image map + particles(red) + endpoints at the mean
    void paintState(RGBImage& img, bool use_distance_map=false);

    //! computes the stats of the filter (mean and covariance matrix)
    void computeStats();

    //! returns the mean of the particles. 
    //! call computeStats of the filter after a predict or an update
    //! to make this method returning a valid value
    inline const  Eigen::Vector3f& mean() const {return _mean;}

    inline const  Eigen::Matrix3f& covariance() const {return _covariance;}
    //! returns the covariance of the particles. 
    //! call computeStats of the filter after a predict or an update
    //! to make this method returning a valid value


    //! Gets the noise coefficients for the translation model.
    //! the standard deviation of the noise to inject is computed
    //! by multiplying this matrix to the absolute values of the control input
    //! thus the noise coeffs represent the contribution of the mixing factors
    //! of the motion components
    //! If the robot does not move the control is 0 and the standard deviations are 0
    inline const Eigen::Matrix3f& noiseCoeffs() const {return _noise_coeffs;}

    //! sets the noise coefficients of the translation model
    inline void setNoiseCoeffs(const Eigen::Matrix3f& nc) { _noise_coeffs = nc;}

    //! read only accessor to the particles
    const ParticleVector& particles() const { return _particles; }

    //! returns the particle resetting status
    //! when on the particles ending up in the unknown are
    //! replaced by samples drawn from the free space.
    //! Good to be on during global localization, bad for tracking
    inline bool particleResetting() const { return _particle_resetting; }
    
    //! enables/disables particle resetting
    inline void setParticleResetting(bool pr)  { _particle_resetting = pr; }

    //! gets the gain for the beam likelihood
    //! the higher this value the more the distance between endpoint and obstacle
    //! will result in a low likelihood;
    inline float likelihoodGain() const {return _likelihood_gain; }
    
    //! sets the likelihood gain
    inline void setLikelihoodGain(float lg)  {_likelihood_gain = lg; }

    //! returns the cumulative likelihood, is a measure of how well the measurements
    //! fit the map
    inline float cumulativeLikelihood() const { return _cumulative_likelihood; }

  protected:
    inline Eigen::Vector2i world2grid(const Eigen::Vector2f p) {
      return Eigen::Vector2i(p.x()*_inverse_resolution, p.y()*_inverse_resolution);
    }

    inline Eigen::Vector2f grid2world(const Eigen::Vector2i p) {
      return Eigen::Vector2f(p.x()*_resolution, p.y()*_resolution);
    }

    //! samples a pose from the free cells, considering the radius of the robot
    Eigen::Vector3f sampleFromFreeSpace();

    // global params
    ParticleVector _particles;
    Vector2fVector _free_cells;
    Eigen::Matrix3f _covariance;
    Eigen::Vector3f _mean;
    float _min_update_translation, _cumulative_translation;
    float _min_update_rotation, _cumulative_rotation;
    bool _particle_resetting;
    double _cumulative_likelihood;
    // map and lookups
    UnsignedCharImage _map;
    float _resolution, _inverse_resolution;
    // lookups extracted from the map (stored for debug)
    IntImage _int_map;
    IntImage _assoc_map;
    DistanceMap _distance_map;
    FloatImage _distances;
    UnsignedCharImage _distance_map_image;
    //last laser valid endpoints
    Vector2fVector _last_endpoints;

    //observation model configuration   
    int _min_valid_points;
    float _min_weight;

    //robot configuration
    float _robot_radius;


    // transition model

    //! prepares the sampling to generate noise for a control
    void prepareSampling(const Eigen::Vector3f& control);

    //! draws a sample from the transition model.
    //! call prepareSampling before to set the control
    //! @param old_state: x_{t-1}
    Eigen::Vector3f sample(const Eigen::Vector3f& old_state);

    Eigen::Matrix3f _noise_coeffs;
    Eigen::Vector3f _last_control;
    Eigen::Vector3f _std_deviations;
    std::tr1::ranlux64_base_01 _random_generator;
    std::tr1::normal_distribution<double> _normal_generator;

    // observation_model
    //! computes the likelihood of an observation, given a particle
    double likelihood(const Eigen::Vector3f& pose, const Vector2fVector& observation);
    float _likelihood_gain; 
  };

}
