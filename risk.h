#ifndef _RISK_H
# define _RISK_H

# define RISK_VERSION_MAJOR 0
# define RISK_VERSION_MINOR 0
# define RISK_VERSION_PATCH 0

# define RISK_VERSION            \
  (                              \
    (RISK_VERSION_MAJOR << 24) | \
    (RISK_VERSION_MINOR << 16) | \
    (RISK_VERSION_PATCH <<  0)   \
  )

# include "rige/rige.h"

#endif