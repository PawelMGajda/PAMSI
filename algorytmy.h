#pragma once

#include <algorithm>

template<typename Typ>
void introWrap(Typ *tab, Typ *tmp, int left, int right, int maxdepth);

template<typename Typ>
void quickWrap(Typ *tab, Typ *tmp, int left, int right, int maxdepth);

template<typename Typ>
void mergeWrap(Typ *tab, Typ *tmp, int left, int right, int maxdepth);
