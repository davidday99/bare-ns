#ifndef _NETCOMMON_H_
#define _NETCOMMON_H_

#define hton16(x) ((((x) & 0xFF) << 8) | (((x) & 0xFF00) >> 8))
#define hton32(x) ((((x) & 0xFF) << 24) | (((x) & 0xFF00) << 8) |  \
                    (((x) & 0xFF0000) >> 8) | (((x) & 0xFF000000) >> 24))

#endif /* _NETCOMMON_H_ */