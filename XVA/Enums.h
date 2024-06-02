#ifndef ENUMS_H_
#define ENUMS_H_

/* contains all Enums*/

enum class RiskType { OWN, CTPY };

enum class TradeType { IrSwap, XccySwap, FxFwd };

enum class LegType { Fixed, Float, MtMFloat };

enum class Currency { USD, EUR };

enum class NotionalExch { YES, NO };

#endif