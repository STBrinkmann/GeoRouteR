// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

#ifdef RCPP_USE_GLOBAL_ROSTREAM
Rcpp::Rostream<true>&  Rcpp::Rcout = Rcpp::Rcpp_cout_get();
Rcpp::Rostream<false>& Rcpp::Rcerr = Rcpp::Rcpp_cerr_get();
#endif

// calculateIsochroneRcpp
DataFrame calculateIsochroneRcpp(IntegerVector from, IntegerVector to, NumericVector cost, NumericVector start_nodes, NumericVector lim, int num_cores);
RcppExport SEXP _GeoRouteR_calculateIsochroneRcpp(SEXP fromSEXP, SEXP toSEXP, SEXP costSEXP, SEXP start_nodesSEXP, SEXP limSEXP, SEXP num_coresSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< IntegerVector >::type from(fromSEXP);
    Rcpp::traits::input_parameter< IntegerVector >::type to(toSEXP);
    Rcpp::traits::input_parameter< NumericVector >::type cost(costSEXP);
    Rcpp::traits::input_parameter< NumericVector >::type start_nodes(start_nodesSEXP);
    Rcpp::traits::input_parameter< NumericVector >::type lim(limSEXP);
    Rcpp::traits::input_parameter< int >::type num_cores(num_coresSEXP);
    rcpp_result_gen = Rcpp::wrap(calculateIsochroneRcpp(from, to, cost, start_nodes, lim, num_cores));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_GeoRouteR_calculateIsochroneRcpp", (DL_FUNC) &_GeoRouteR_calculateIsochroneRcpp, 6},
    {NULL, NULL, 0}
};

RcppExport void R_init_GeoRouteR(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}