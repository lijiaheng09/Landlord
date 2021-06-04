#include "card.h"
#include "main.h"

typedef double VL;

extern int fl,qj;

inline VL PASSVL();
inline VL SINGLEVL(int x);
inline VL PAIRVL(int x);
inline VL STRAIGHTVL(int l, int len);
inline VL STRAIGHT2VL(int l,int len);
inline VL TRIPLETVL(int x);
inline VL TRIPLET1VL(int x);
inline VL TRIPLET2VL(int x);
inline VL BOMBVL(int x);
inline constexpr VL QUADRUPLE2VL(int x);
inline constexpr VL QUADRUPLE4VL(int x);
inline constexpr VL PLANEVL(int x);
inline constexpr VL PLANE1VL(int x);
inline constexpr VL PLANE2VL(int x);
inline constexpr VL SSHUTTLEVL(int x);
inline constexpr VL SSHUTTLE2VL(int x);
inline constexpr VL SSHUTTLE4VL(int x);
inline VL ROCKETVL();
inline constexpr VL INVALIDVL();
