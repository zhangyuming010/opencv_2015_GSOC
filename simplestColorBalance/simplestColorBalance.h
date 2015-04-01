#ifndef SIMPLEST_COLOR_BALANCE
#define SIMPLEST_COLOR_BALANCE

#include"opencv2/core/core.hpp"
/*@brief compute accumulate histogram for single channel
 *@param _imSingle [in] Input signle channel image
 *@param hist [out] Accumulate histogram
*/
void computeAccHist(cv::Mat _imSingle, cv::Mat& hist);

/*@brief saturate pixels and compute linear mapping
 *@param _im [in] Input single channel image
 *@param _hist [in] The accumulate histogram
 *@param _mapping [out] The output linear mapping
 *@param s1 [in] First saturate parameter
 *@param s2 [in] Second saturate parameter
*/
void computeMapping(const cv::InputArray _imSingle, const cv::InputArray _hist, cv::OutputArray _mapping, float s1, float s2);

/*@brief remap an image using mapping computed by computeMapping
 *@param _mapping [in] mapping
 *@param _imSingle [in] Input image
 *@param _outSingle [out] Output image
*/
void remapPixValue(const cv::InputArray _mapping, cv::InputArray _imSingle, cv::OutputArray _outSingle);

/*@brief The main function for simplest color banlance
 *@param _im [in] The input 3 channel image
 *@param _imOut [out] Th output image
 *@param s1 [in] First saturate parameter
 *@param s2 [in] Second saturate parameter
*/
void simplestColorBanlance(cv::InputArray _im, cv::OutputArray _imOut, float s1, float s2);

#endif