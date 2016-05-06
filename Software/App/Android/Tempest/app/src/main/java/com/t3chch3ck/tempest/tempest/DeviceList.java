/*
 * Copyright (C) 2015, Zentri, Inc. All Rights Reserved.
 *
 * The Zentri BLE Android Libraries and Zentri BLE example applications are provided free of charge
 * by Zentri. The combined source code, and all derivatives, are licensed by Zentri SOLELY for use
 * with devices manufactured by Zentri, or devices approved by Zentri.
 *
 * Use of this software on any other devices or hardware platforms is strictly prohibited.
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

package com.t3chch3ck.tempest.tempest;

import android.widget.ArrayAdapter;
import android.widget.ListView;

public class DeviceList
{
    ArrayAdapter<String> mDeviceAdapter;

    public DeviceList(ArrayAdapter<String> adapter, ListView view)
    {
        mDeviceAdapter = adapter;
        view.setAdapter(mDeviceAdapter);
        mDeviceAdapter.clear();
    }

    public void add(String name)
    {
        String deviceName = findDeviceWithName(name);

        if (deviceName == null)//only add if not already in the list
        {
            if (name != null)
            {
                mDeviceAdapter.add(name);
            }
        }
    }

    public String get(int position)
    {
        return mDeviceAdapter.getItem(position);
    }

    public void remove(String name)
    {
        String toRemove = findDeviceWithName(name);

        if (toRemove != null)
        {
            mDeviceAdapter.remove(toRemove);
        }
    }

    public void clear()
    {
        mDeviceAdapter.clear();
    }

    public String findDeviceWithName(String name)
    {
        String deviceName = null;
        int count = mDeviceAdapter.getCount();
        for (int i=0; i<count; i++)
        {
            String nextName = mDeviceAdapter.getItem(i);

            if (nextName.equals(name))
            {
                deviceName = nextName;
                break;
            }
        }
        return deviceName;
    }
}
