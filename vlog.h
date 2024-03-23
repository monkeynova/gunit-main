#ifndef VLOG_H
#define VLOG_H

#include "absl/log/log.h"
#include "absl/log/vlog_is_on.h"

// TODO(@monkeynova): Remove when part of absl/log public API.

#define VLOG_IF(s, cond) LOG_IF(INFO, VLOG_IS_ON(s) && (cond))
#define VLOG_IF_EVERY_N(s, cond, n) LOG_IF(INFO, VLOG_IS_ON(s) && (cond), n)

#define DVLOG_IF(s, cond) DLOG_IF(INFO, VLOG_IS_ON(s) && (cond))
#define DVLOG_IF_EVERY_N(s, cond, n) DLOG_IF(INFO, VLOG_IS_ON(s) && (cond), n)

#define RETURN_IF_ERROR(arg) \
  {                          \
    absl::Status st = arg;   \
    if (!st.ok()) return st; \
  }

#define ASSIGN_OR_RETURN_CONCAT(x, y) ASSIGN_OR_RETURN_CONCAT_(x, y)
#define ASSIGN_OR_RETURN_CONCAT_(x, y) x##y

#define ASSIGN_OR_RETURN(lhs, rhs)                                           \
  ASSIGN_OR_RETURN_IMPL(ASSIGN_OR_RETURN_CONCAT(_status_or_, __LINE__), lhs, \
                        rhs)

#define ASSIGN_OR_RETURN_IMPL(statusor_var, lhs, rhs)   \
  auto statusor_var = (rhs);                            \
  if (!statusor_var.ok()) return statusor_var.status(); \
  lhs = *std::move(statusor_var)

#define CHECK_NEAR(f1, f2, epsilon) \
  CHECK(abs((f1) - (f2)) < epsilon) << f1 << " ~ " << f2

#define DCHECK_NEAR(f1, f2, epsilon) \
  DCHECK(abs((f1) - (f2)) < epsilon) << f1 << " ~ " << f2

#endif  // VLOG_H
