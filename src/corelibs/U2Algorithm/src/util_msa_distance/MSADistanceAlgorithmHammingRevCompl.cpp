/**
 * UGENE - Integrated Bioinformatics Tools.
 * Copyright (C) 2008-2016 UniPro <ugene@unipro.ru>
 * http://ugene.net
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include "MSADistanceAlgorithmHammingRevCompl.h"

#include <U2Core/AppContext.h>
#include <U2Core/DNATranslation.h>
#include <U2Core/MAlignment.h>
#include <U2Core/TextUtils.h>
#include <U2Core/U2OpStatusUtils.h>
#include <U2Core/U2SafePoints.h>


namespace U2 {

MSADistanceAlgorithmFactoryHammingRevCompl::MSADistanceAlgorithmFactoryHammingRevCompl(QObject* p)
: MSADistanceAlgorithmFactory(BuiltInDistanceAlgorithms::HAMMING_REVCOMPL_ALGO, DistanceAlgorithmFlag_Nucleic, p)
{

}


QString MSADistanceAlgorithmFactoryHammingRevCompl::getDescription() const {
    return tr("Based on Hamming distance between two sequences");
}

QString MSADistanceAlgorithmFactoryHammingRevCompl::getName() const {
    return tr("Hamming reverse-complement");
}


MSADistanceAlgorithm* MSADistanceAlgorithmFactoryHammingRevCompl::createAlgorithm(const MAlignment& ma, QObject* ) {
    return new MSADistanceAlgorithmHammingRevCompl(this, ma);
}

//////////////////////////////////////////////////////////////////////////
// Algorithm

void MSADistanceAlgorithmHammingRevCompl::run() {
    DNATranslation* compTT = AppContext::getDNATranslationRegistry()->
        lookupComplementTranslation(ma.getAlphabet());

    assert (compTT != NULL);

    DNATranslation* trans = compTT ;
    int nSeq = ma.getNumRows();
    MAlignment revtransl;
    revtransl.setAlphabet(ma.getAlphabet());
    U2OpStatus2Log os;
    for (int i = 0; i < nSeq; i++) {
        if (isCanceled()) {
            return;
        }
        QByteArray arr = ma.getRow(i).toByteArray(ma.getLength(), os);
        trans->translate(arr.data(), arr.length());
        TextUtils::reverse(arr.data(), arr.length());

        revtransl.addRow(ma.getRow(i).getName(), arr, os);

        CHECK_OP_EXT(os, setError(tr("An unexpected error has occurred during running"
                                      " the Hamming reverse-complement algorithm.")),);
    }

    for (int i = 0; i < nSeq; i++) {
        for (int j = i; j < nSeq; j++) {
            int sim = 0;
            for (int k = 0; k < ma.getLength(); k++) {
                if (isCanceled()) {
                    return;
                }
                if (ma.charAt(i, k) == revtransl.charAt(j, k)) {
                    sim++;
                }
            }
            lock.lock();
            setDistanceValue(i, j, sim);
            lock.unlock();
        }
        stateInfo.setProgress(i * 100 / nSeq);
    }
}

} //namespace
