#ifndef __7ed6b0d4d9244155819c464fc4eb9bbb
#define __7ed6b0d4d9244155819c464fc4eb9bbb

#include <lf/base/ref_el.h>
#include <Eigen/Eigen>
#include <memory>

namespace lf::geometry {

class Geometry {
public:

  using coord_t = Eigen::VectorXd;
  using dim_t = base::RefEl::dim_t;

  /**
   * @brief Dimension of the domain of this mapping.
   */
  virtual dim_t DimLocal() const = 0;

  /**
   * @brief Dimension of the image of this mapping.
   */
  virtual dim_t DimGlobal() const = 0;

  /**
   * @brief The Reference element that defines the domain of this mapping.
   */
  virtual base::RefEl RefEl() const = 0;

  /**
   * @brief Map a number of points in local coordinates into the global 
   *        coordinate system.
   * @param local A Matrix of size `DimLocal() x numPoints` that contains
   *              in its columns the coordinates of the points at which the 
   *              mapping function should be evaluated.
   * @return A Matrix of size `DimGlobal() x numPoints` that contains the mapped
   *         points as column vectors. Here `numPoints` is the number of columns
   *         of the matrix passed in the `local` argument.
   *
   * This method provides a complete description of the shape of an entity 
   * through a parameterization over the corresponding reference element = 
   * parameter domain. The method takes as arguments a number of coordinate
   * vectors of points in the reference element. For the sake of efficiency, 
   * these coordinate vectors are passed as the columns of a dynamic matrix type
   * as supplied by Eigen. 
   */
  virtual Eigen::MatrixXd Global(const Eigen::MatrixXd& local) const = 0;

  /**
   * @brief Evaluate the jacobian of the mapping simultaneously at `numPoints`
   *        points.
   * @param local A Matrix of size `DimLocal x numPoints` that contains the 
   *              evaluation points as column vectors
   * @return A Matrix of size `DimGlobal() x (DimLocal() * numPoints)` that contains
   *         the jacobians at the evaluation points.
   *
   * This method allows access to the derivative of the parametrization mapping 
   * in a number of points, passed as the columns of a dynamic matrix. The derivative 
   * of the parametrization in a point is a Jacobian matrix of size 
   * `DimGlobal() x DimLocal()'. For the sake of efficiency, these matrices are 
   * stacked horizontally and returned as one big dynamic matrix. Use Eigen's 
   * `block()' method of `Eigen::MatrixXd` to extract the individual Jacobians from
   * the returned matrix. 
   */
  virtual Eigen::MatrixXd Jacobian(const Eigen::MatrixXd& local) const = 0;

  /**
   * @brief Evaluate the Jacobian * Inverse Gramian (\f$ J (J^T J)^{-1}\f$)
   *        simulatanesouly at `numPoints`.
   * @param local A Matrix of size `DimLocal() x numPoints` that contains the
   *              evaluation points as column vectors.
   * @return A Matrix of size `DimGlobal() x (DimLocal() * numPoints)` that
   *         contains the Jacobian multiplied with the Inverse Gramian 
   *         ( \f$ J (J^T J)^{-1}\f$) at every evaluation point.
   *         
   * @note If `DimLocal() == DimGlobal()` then \f$ J (J^T J)^{-1} = J^{-T} \f$,
   *       i.e. this method returns the inverse of the transposed jacobian.
   */
  virtual Eigen::MatrixXd
    JacobianInverseGramian(const ::Eigen::MatrixXd& local) const = 0;

  /**
   * @brief The integration element (factor appearing in integral transformation
   *        formula, see below) at number of evaluation points (specified in
   *        local coordinates).
   * @param local A Matrix of size `DimLocal() x numPoints` that contains the 
   *              evaluation points (in local coordinates) as column vectors.
   * @return A Vector of size `numPoints x 1` that contains the integration
   *         elements at every evaluation point.
   *         
   * For a transformation \f$ \Phi : K \mapsto R^{\text{DimGlobal}}\f$ with Jacobian 
   * \f$ D\Phi : K \mapsto R^{\text{DimGlobal} \times \text{DimLocal}} \f$ the integration
   * element \f$ g \f$ at point \f$ \xi \in K \f$ is defined by
   * \f[
   * g(\xi) := \sqrt{\mathrm{det}\left|D\Phi^T(\xi) D\Phi(\xi) \right|}
   * \f]
   */
  virtual Eigen::VectorXd
    IntegrationElement(const Eigen::MatrixXd& local) const = 0;


  /**
   * @brief Construct a new Geometry() object that describes the geometry of 
   *        the `i`-th sub-entity with codimension=`codim`
   * @param codim The codimension of the sub-entity (w.r.t. `DimLocal()`)
   * @param i The zero-based index of the sub-entity.
   * @return A new Geometry object that describes the geometry of the specified
   *         sub-entity.
   *         
   * Let \f$ \mathbf{\Phi} : K \mapsto \mathbb{R}^\text{DimGlobal} \f$ be the mapping 
   * of this Geometry object and let \f$ \mathbf{\xi} : \mathbb{R}^{\text{DimLocal}-codim} \mapsto K\f$
   * be the first-order mapping that
   * maps the reference element `RefEl().SubType(codim,i)` to the `i`-th 
   * sub-entity of `RefEl()`. I.e. for every node \f$ \mathbf{n_j} \f$ of `RefEl().SubType(codim,i)`
   * it holds that \f$ \mathbf{\xi}(\mathbf{n_j}) = \f$ 
   * `RefEl().NodeCoords(RefEl().SubSubEntity2SubEntity(codim, i, DimLocal()-codim, j))`.
   * 
   * Then the geometry element returned by this method describes exactly the mapping
   * \f$ \mathbf{\Phi} \circ \mathbf{\xi} \f$
   */
  virtual std::unique_ptr<Geometry> subGeometry(dim_t codim, dim_t i) const = 0;

  /**
   * @brief Virtual destructor
   */
  virtual ~Geometry() = default;
};

} // namespace lf::mesh

#endif  // __7ed6b0d4d9244155819c464fc4eb9bbb