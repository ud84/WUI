//
// Copyright (c) 2023 Anton Golovkov (udattsk at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/ud84/wui
//

#pragma once

#include <string>

namespace wui
{

/// ISO 639-2/B lang codes
enum class locale_type
{
    abk,
    aar,
    afr,
    aka,
    alb,
    amh,
    ara,
    arg,
    arm,
    asm_,
    ava,
    ave,
    aym,
    aze,
    bam,
    bak,
    baq,
    bel,
    ben,
    bis,
    bos,
    bre,
    bul,
    bur,
    cat,
    cha,
    che,
    nya,
    chi,
    chu,
    chv,
    cor,
    cos,
    cre,
    hrv,
    cze,
    dan,
    div,
    dut,
    dzo,
    eng,
    epo,
    est,
    ewe,
    fao,
    fij,
    fin,
    fre,
    fry,
    ful,
    gla,
    glg,
    lug,
    geo,
    ger,
    gre,
    kal,
    grn,
    guj,
    hat,
    hau,
    heb,
    her,
    him,
    hmo,
    hun,
    ice,
    ido,
    ibo,
    ind,
    ina,
    ile,
    iku,
    ipk,
    gle,
    ita,
    jpn,
    jav,
    kan,
    kau,
    kas,
    kaz,
    khm,
    kik,
    kin,
    kir,
    kom,
    kon,
    kor,
    kua,
    kur,
    lao,
    lat,
    lav,
    lim,
    lin,
    lit,
    lut,
    ltz,
    mtz,
    mac,
    mlg,
    may,
    mal,
    mit,
    glv,
    mao,
    mar,
    mah,
    mon,
    nau,
    nav,
    nde,
    nbl,
    ndo,
    nep,
    nor,
    nob,
    nno,
    ili,
    oci,
    oji,
    ori,
    orm,
    oss,
    pli,
    pus,
    per,
    pol,
    por,
    pan,
    que,
    rum,
    roh,
    run,
    rus,
    sme,
    smo,
    sag,
    san,
    srd,
    srp,
    sna,
    snd,
    sin,
    slo,
    slv,
    som,
    sot,
    spa,
    sun,
    swa,
    ssw,
    swe,
    tgl,
    tah,
    tgk,
    tam,
    tat,
    tel,
    tha,
    tib,
    tir,
    ton,
    tso,
    tsn,
    tur,
    tuk,
    twi,
    uig,
    ukr,
    urd,
    uzb,
    ven,
    vie,
    vol,
    wln,
    wel,
    wol,
    xho,
    yid,
    yor,
    zha,
    zul
};

std::string_view str(locale_type);

}
