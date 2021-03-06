#ifndef _LDAPLUSPLUS_EM_UNSUPERVISEDESTEP_HPP_
#define _LDAPLUSPLUS_EM_UNSUPERVISEDESTEP_HPP_

#include "ldaplusplus/em/AbstractEStep.hpp"

namespace ldaplusplus {
namespace em {


/**
 * UnsupervisedEStep implements the classic LDA expectation step.
 *
 * For each document passed in UnsupervisedEStep::doc_e_step a factorized
 * variational distribution is computed with Dirichlet parameter \f$\gamma\f$
 * and multinomial parameters \f$\phi\f$. The distribution is computed in such
 * a way so that a lower bound of the probability of generating the document
 * given the model parameters (the topics that is) is maximized.
 *
 * See UnsupervisedEStep::doc_e_step for the mathematics.
 *
 * [1] Blei, David M., Andrew Y. Ng, and Michael I. Jordan. "Latent dirichlet
 *     allocation." Journal of machine Learning research 3.Jan (2003):
 *     993-1022.
 */
template <typename Scalar>
class UnsupervisedEStep : public AbstractEStep<Scalar>
{
    typedef Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> MatrixX;
    typedef Eigen::Matrix<Scalar, Eigen::Dynamic, 1> VectorX;

    public:
        /**
         * @param e_step_iterations  The max number of times to alternate
         *                           between maximizing for \f$\gamma\f$ and
         *                           for \f$\phi\f$.
         * @param e_step_tolerance   The minimum relative change in the
         *                           variational parameter \f$\gamma\f$.
         * @param compute_likelihood The percentage of documents to compute
         *                           likelihood for (1.0 means compute for
         *                           every document)
         * @param random_state       An initial seed value for any random
         *                           numbers needed
         */
        UnsupervisedEStep(
            size_t e_step_iterations = 10,
            Scalar e_step_tolerance = 1e-2,
            Scalar compute_likelihood = 1.0,
            int random_state = 0
        );

        /**
         * Maximize the ELBO w.r.t. to \f$\phi\f$ and \f$\gamma\f$.
         *
         * The following steps are the mathematics that are implemented where
         * \f$\beta\f$ are the topics, \f$i\f$ is the topic subscript, \f$n\f$
         * is the word subscript, \f$w_n\f$ is n-th word vocabulary index,
         * \f$\alpha\f$ is the Dirichlet prior and finally \f$\Psi(\cdot)\f$ is
         * the first derivative of the \f$\log \Gamma\f$ function.
         *
         * 1. Repeat following steps until convergence
         * 2. \f$\phi_{ni} \propto \beta_{iw_n} \exp(\Psi(\gamma_i)) \f$
         * 3. \f$\gamma_i = \alpha_i + \sum_n^N \phi_{ni} \f$
         *
         * @param doc        A single document
         * @param parameters An instance of class Parameters, which
         *                   contains all necessary model parameters 
         *                   for e-step's implementation
         * @return           The variational parameters for the current
         *                   model, after e-step is completed
         */
        virtual std::shared_ptr<parameters::Parameters> doc_e_step(
            const std::shared_ptr<corpus::Document> doc,
            const std::shared_ptr<parameters::Parameters> parameters
        ) override;

    private:
        // The maximum number of iterations in E-step.
        size_t e_step_iterations_;
        // The convergence tolerance for the maximazation of the ELBO w.r.t.
        // phi and gamma in E-step
        Scalar e_step_tolerance_;
        // Compute the likelihood of that many documents (pecentile)
        Scalar compute_likelihood_;
};

}  // namespace em
}  // namespace ldaplusplus

#endif // _LDAPLUSPLUS_EM_UNSUPERVISEDESTEP_HPP_
