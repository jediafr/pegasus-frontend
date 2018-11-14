// Pegasus Frontend
// Copyright (C) 2017-2018  Mátyás Mustoha
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.


import QtQuick 2.0
import QtTest 1.11


TestCase {
    name: "deleting"
    when: windowShown


    SignalSpy {
        id: changed
        target: keyEditor
        signalName: "keysChanged"
    }


    function hasKey(event, key) {
        return keyEditor.keyCodesOf(event).indexOf(key) > -1;
    }

    function init() {
        keyEditor.resetKeys();
        changed.clear();
    }

    function test_del() {
        var test_key = Qt.Key_Return;
        var key_count = keyEditor.keyCodesOf(0).length;
        compare(hasKey(0, test_key), true);

        keyEditor.delKey(0, test_key);

        tryCompare(changed, "count", 1);
        tryCompare(keyEditor.keyCodesOf(0), "length", key_count - 1);
        compare(hasKey(0, test_key), false);
    }

    function test_invalid() {
        var key_count = keyEditor.keyCodesOf(0).length;

        keyEditor.delKey(0, 0);

        tryCompare(changed, "count", 0);
        tryCompare(keyEditor.keyCodesOf(0), "length", key_count);
    }
}
