package com.t3chch3ck.tempest.tempest;

import android.Manifest;
import android.app.AlertDialog;
import android.app.Dialog;
import android.bluetooth.BluetoothAdapter;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.pm.PackageManager;
import android.content.res.ColorStateList;
import android.content.res.Resources;
import android.graphics.Color;
import android.os.Handler;
import android.os.IBinder;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v4.content.LocalBroadcastManager;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;

import android.widget.AdapterView;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.zentri.zentri_ble_command.ErrorCode;
import com.zentri.zentri_ble_command.ZentriOSBLEManager;

import java.util.regex.Pattern;

public class MainActivity extends AppCompatActivity implements TempestSettingsFragment.OnMaxRofEyesOnChanged {

    /**
     * The {@link android.support.v4.view.PagerAdapter} that will provide
     * fragments for each of the sections. We use a
     * {@link FragmentPagerAdapter} derivative, which will keep every
     * loaded fragment in memory. If this becomes too memory intensive, it
     * may be best to switch to a
     * {@link android.support.v4.app.FragmentStatePagerAdapter}.
     */
    private SectionsPagerAdapter mSectionsPagerAdapter;

    /**
     * The {@link ViewPager} that will host the section contents.
     */
    private ViewPager mViewPager;

    //***********************************************

    private static final String TAG = MainActivity.class.getSimpleName();

    private static final String LOC_PERM = Manifest.permission.ACCESS_COARSE_LOCATION;
    private static final int BLE_ENABLE_REQ_CODE = 1;
    private static final int LOC_ENABLE_REQ_CODE = 2;

    private static final long SCAN_PERIOD = 30000;
    private static final long CONNECT_TIMEOUT_MS = 10000;

    private static final String PATTERN_MAC_ADDRESS = "(\\p{XDigit}{2}:){5}\\p{XDigit}{2}";

    //private SmoothProgressBar mScanProgressBar;
    private Dialog mConnectProgressDialog;
    private DeviceList mDeviceList;
    private Button mScanButton;

    private Handler mHandler;
    private Runnable mStopScanTask;
    private Runnable mConnectTimeoutTask;

    private ZentriOSBLEManager mZentriOSBLEManager;
    private boolean mConnecting = false;
    private boolean mConnected = false;

    private String mCurrentDeviceName;

    private ServiceConnection mConnection;
    private ZentriOSBLEService mService;
    private boolean mBound = false;

    private LocalBroadcastManager mLocalBroadcastManager;
    private BroadcastReceiver mBroadcastReceiver;
    private IntentFilter mReceiverIntentFilter;

    private Dialog mLocationEnableDialog;
    private Dialog mPermissionRationaleDialog;

    private TempestDisplayFragment tempestDisplayFragment = null;
    private TempestSettingsFragment tempestSettingsFragment = null;

    private FloatingActionButton fab;

    //***********************************************


    public void onMaxRofEyesOnChanged(double value) {
        Log.i(TAG, "onMaxRofEyesOnChanged " + value);
        sendDouble('a', ZentriOSBLEService.frequencyToTicks(value));
        sendRequest('a');
    }

    public void onMaxRofEyesOffChanged(double value) {
        Log.i(TAG, "onMaxRofEyesOffChanged " + value);
        sendDouble('b', ZentriOSBLEService.frequencyToTicks(value));
        sendRequest('b');
    }

    public void onRampActivationChanged(double value) {
        Log.i(TAG, "onRampActivationChanged " + value);
        sendDouble('c', ZentriOSBLEService.frequencyToTicks(value));
        sendRequest('c');
    }

    public void onDwellChanged(int value) {
        Log.i(TAG, "onDwellChanged " + value);
        sendDouble('e', ZentriOSBLEService.frequencyToTicks(1000.0/(double)value));
        sendRequest('e');
    }

    public void onRampActivationShotsChanged(int value) {
        Log.i(TAG, "onRampActivationShotsChanged " + value);
        sendDouble('d', value);
        sendRequest('d');
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);


        // Set up the ViewPager with the sections adapter.
        mViewPager = (ViewPager) findViewById(R.id.container);

        // Create the adapter that will return a fragment for each of the three
        // primary sections of the activity.
        mSectionsPagerAdapter = new SectionsPagerAdapter(getSupportFragmentManager());

        mViewPager.setAdapter(mSectionsPagerAdapter);

        /*mViewPager.addOnPageChangeListener(new ViewPager.OnPageChangeListener() {
            @Override
            public void onPageScrolled(int position, float positionOffset, int positionOffsetPixels) {
                Log.d(TAG, "onPageScrolled " + position);
            }

            @Override
            public void onPageSelected(int position) {
                Log.d(TAG, "onPageSelected " + position);
            }

            @Override
            public void onPageScrollStateChanged(int state) {

            }
        });*/




        fab = (FloatingActionButton) findViewById(R.id.fab);

        fab.setBackgroundTintList(ColorStateList.valueOf(Color.GRAY));

        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //sendRequest('a');
                //Log.i(TAG, "mZentriOSBLEManager.getMode " + mZentriOSBLEManager.getMode());
                /*Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();*/
            }
        });


        initDeviceList();
        initBroadcastManager();
        initServiceConnection();
        initBroadcastReceiver();
        initReceiverIntentFilter();

        startService(new Intent(this, ZentriOSBLEService.class));

        mHandler = new Handler();

        mStopScanTask = new Runnable()
        {
            @Override
            public void run()
            {
                stopScan();
            }
        };

        mConnectTimeoutTask = new Runnable()
        {
            @Override
            public void run()
            {
                runOnUiThread(new Runnable()
                {
                    @Override
                    public void run()
                    {
                        dismissDialog(mConnectProgressDialog);
                        showErrorDialog(R.string.con_timeout_message, false);
                        mConnecting = false;
                        mConnected = false;
                        fab.setBackgroundTintList(ColorStateList.valueOf(Color.GRAY));
                        if(mZentriOSBLEManager != null && mZentriOSBLEManager.isConnected())
                        {
                            mZentriOSBLEManager.disconnect(ZentriOSBLEService.DISABLE_TX_NOTIFY);
                        }
                    }
                });
            }
        };

    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            openAboutDialog();
            return true;
        } else {

            return super.onOptionsItemSelected(item);
        }
    }




    @Override
    protected void onStart()
    {
        super.onStart();

        //mDeviceList.clear();
        mConnected = false;
        mConnecting = false;
        fab.setBackgroundTintList(ColorStateList.valueOf(Color.GRAY));

        Intent intent = new Intent(this, ZentriOSBLEService.class);
        bindService(intent, mConnection, Context.BIND_AUTO_CREATE);

        mLocalBroadcastManager.registerReceiver(mBroadcastReceiver, mReceiverIntentFilter);
    }

    @Override
    protected void onResume()
    {
        super.onResume();
    }

    @Override
    protected void onPause()
    {
        super.onPause();
    }

    @Override
    protected void onStop()
    {
        mHandler.removeCallbacks(mStopScanTask);

        //ensure dialogs are closed
        dismissDialog(mConnectProgressDialog);
        dismissDialog(mLocationEnableDialog);
        dismissDialog(mPermissionRationaleDialog);

        if (mBound)
        {
            mLocalBroadcastManager.unregisterReceiver(mBroadcastReceiver);
            unbindService(mConnection);
            mBound = false;
        }

        super.onStop();
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();

        stopService(new Intent(this, ZentriOSBLEService.class));
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data)
    {
        if (requestCode == BLE_ENABLE_REQ_CODE)
        {
            mService.initTruconnectManager();//try again
            if (mZentriOSBLEManager.isInitialised())
            {
                if (requirementsMet())
                {
                    startScan();
                }
            }
            else
            {
                showErrorDialog(R.string.init_fail_msg, true);
            }
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String permissions[], int[] grantResults)
    {
        switch (requestCode)
        {
            case LOC_ENABLE_REQ_CODE:
            {
                // If request is cancelled, the result arrays are empty.
                if (grantResults.length > 0 &&
                        grantResults[0] == PackageManager.PERMISSION_GRANTED)
                {
                    if (requirementsMet())
                    {
                        startScan();
                    }
                }
                else
                {
                    //show unrecoverable error dialog
                    showErrorDialog(R.string.error_permission_denied, true);
                }
            }
        }
    }


    /**
     * A {@link FragmentPagerAdapter} that returns a fragment corresponding to
     * one of the sections/tabs/pages.
     */
    public class SectionsPagerAdapter extends FragmentPagerAdapter {

        public SectionsPagerAdapter(FragmentManager fm) {
            super(fm);
        }

        @Override
        public Fragment getItem(int position) {
            // getItem is called to instantiate the fragment for the given page.
            // Return a PlaceholderFragment (defined as a static inner class below).
            if (position == 0) {
                tempestDisplayFragment = new TempestDisplayFragment();
                Log.i(TAG, "TempestDisplayFragment");
                return tempestDisplayFragment;
            } else if (position ==1 ) {
                tempestSettingsFragment = new TempestSettingsFragment();
                Log.i(TAG, "TempestSettingsFragment");
                return tempestSettingsFragment;
            } else

            return null;
        }

        @Override
        public int getCount() {
            return 2;
        }

        @Override
        public CharSequence getPageTitle(int position) {
            switch (position) {
                case 0:
                    return "SECTION 1";
                case 1:
                    return "SECTION 2";
            }
            return null;
        }

    }



    private void initProgressBar()
    {
        //mScanProgressBar = (SmoothProgressBar) findViewById(R.id.progressBar);
        //mScanProgressBar.setVisibility(View.VISIBLE);
    }

    private void initScanButton()
    {
        /*mScanButton = (Button) findViewById(R.id.scanButton);
        mScanButton.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                mDeviceList.clear();
                startScan();
            }
        });*/
    }

    private void initDeviceList()
    {
        //ListView deviceListView = (ListView) findViewById(R.id.listView);
        //ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, R.layout.listitem, R.id.textView);

        //initialiseListviewListener(deviceListView);
        //mDeviceList = new DeviceList(adapter, deviceListView);
    }

    private void initServiceConnection()
    {
        mConnection = new ServiceConnection()
        {
            @Override
            public void onServiceConnected(ComponentName className, IBinder service)
            {
                ZentriOSBLEService.LocalBinder binder = (ZentriOSBLEService.LocalBinder) service;
                mService = binder.getService();
                mBound = true;

                mZentriOSBLEManager = mService.getManager();

                //if requirements not met, action will already be taken
                if (requirementsMet())
                {
                    startScan();
                }
            }

            @Override
            public void onServiceDisconnected(ComponentName arg0)
            {
                mBound = false;
            }
        };
    }

    private void initBroadcastReceiver()
    {
        mBroadcastReceiver = new BroadcastReceiver()
        {
            @Override
            public void onReceive(Context context, Intent intent)
            {
                // Get extra data included in the Intent
                String action = intent.getAction();


                switch (action)
                {

                    case ZentriOSBLEService.ACTION_STRING_DATA_WRITE:
                        Log.i(TAG, "ACTION_STRING_DATA_WRITE ");
                        break;

                    case ZentriOSBLEService.ACTION_SCAN_RESULT:
                        String name = ZentriOSBLEService.getData(intent);

                        //dont show devices with no name (mac addresses)
                        if (name != null && !Pattern.matches(PATTERN_MAC_ADDRESS, name))
                        {
                            addDeviceToList(name);
                            Log.i(TAG, "ACTION_SCAN_RESULT " + name);

                            if (name.equals("Tempest#")) {
                                mZentriOSBLEManager.stopScan();
                                mZentriOSBLEManager.connect(name);

                                //showToast("Connecting.....", Toast.LENGTH_SHORT);
                            }
                        }
                        break;

                    case ZentriOSBLEService.ACTION_CONNECTED:
                        String deviceName = ZentriOSBLEService.getData(intent);

                        mConnected = true;
                        fab.setBackgroundTintList(ColorStateList.valueOf(Color.GREEN));

                        mHandler.removeCallbacks(mConnectTimeoutTask); //cancel timeout timer


                        dismissDialog(mConnectProgressDialog);
                        showToast("Connected to Tempest", Toast.LENGTH_SHORT);
                        Log.d(TAG, "Connected to " + deviceName);

                        sendRequest('a');
                        sendRequest('b');
                        sendRequest('c');
                        sendRequest('d');
                        sendRequest('e');
                        // startDeviceInfoActivity();
                        break;

                    case ZentriOSBLEService.ACTION_DISCONNECTED:
                        mConnected = false;
                        Log.i(TAG, "Disconnected ");
                        fab.setBackgroundTintList(ColorStateList.valueOf(Color.GRAY));
                        break;

                    case ZentriOSBLEService.TEMPEST_ACTION_UPDTAED:
                        //Log.i(TAG, "TEMPEST_ACTION_UPDTAED ");
                        tempestDisplayFragment.updateDisplay(ZentriOSBLEService.getTempestState());
                        //tempestSettingsFragment.updateSettings(ZentriOSBLEService.getTempestState());
                        String extra  = intent.getStringExtra(ZentriOSBLEService.TEMPEST_EXTRA_DATA);

                        if (extra!=null) {
                            if (extra.equals(ZentriOSBLEService.TEMPEST_EXTRA_PARAM)) {
                                tempestSettingsFragment.updateMaxRofEyesOn(ZentriOSBLEService.ticksToFrequency(ZentriOSBLEService.getTempestState().getMaxRofEyesOn()));
                                tempestSettingsFragment.updateMaxRofEyesOff(ZentriOSBLEService.ticksToFrequency(ZentriOSBLEService.getTempestState().getMaxRofEyesOff()));
                                tempestSettingsFragment.updateRampActivation(ZentriOSBLEService.ticksToFrequency(ZentriOSBLEService.getTempestState().getRampKickinRof()));

                                tempestSettingsFragment.updateRampActtivationShots(ZentriOSBLEService.getTempestState().getRampKickinShots());
                                tempestSettingsFragment.updateDwell((int)Math.round(1000.0 / ZentriOSBLEService.ticksToFrequency(ZentriOSBLEService.getTempestState().getDwell())));
                            }
                        }
                        break;

                    case ZentriOSBLEService.ACTION_ERROR:
                        ErrorCode errorCode = ZentriOSBLEService.getErrorCode(intent);
                        //handle errors
                        if (errorCode == ErrorCode.CONNECT_FAILED)
                        {
                            Log.i(TAG, "CONNECT_FAILED ");
                            if (!mConnected && mConnecting)
                            {
                                mConnecting = false;//allow another attempt to connect
                                dismissDialog(mConnectProgressDialog);
                            }
                            else
                            {
                                mConnected = false;
                                fab.setBackgroundTintList(ColorStateList.valueOf(Color.GRAY));
                            }

                            showErrorDialog(R.string.con_err_message, false);
                        }
                        break;
                }
            }
        };
    }

    public void sendRequest(char command) {
        String commandString = "";

        commandString += '+';   // start of packet
        commandString += 'g';   // get param
        commandString += command;
        commandString += '#';   // end of packet

        if (mZentriOSBLEManager.isConnected()) {
            Log.i(TAG, "sendRequest " + commandString);
            mZentriOSBLEManager.writeData(commandString);
        }

    }

    public void sendDouble(char command, int value) {
        String commandString = "";
        String intString = String.format("%06d", value);

        commandString += '+';   // start of packet
        commandString += 's';   // get param
        commandString += command;
        commandString += intString;
        commandString += '#';   // end of packet

        if (mZentriOSBLEManager.isConnected()) {
            Log.i(TAG, "sendDouble " + commandString);
            mZentriOSBLEManager.writeData(commandString);
        }

    }

    public void initBroadcastManager()
    {
        mLocalBroadcastManager = LocalBroadcastManager.getInstance(getApplicationContext());
    }

    public void initReceiverIntentFilter()
    {
        mReceiverIntentFilter = new IntentFilter();
        mReceiverIntentFilter.addAction(ZentriOSBLEService.ACTION_SCAN_RESULT);
        mReceiverIntentFilter.addAction(ZentriOSBLEService.ACTION_CONNECTED);
        mReceiverIntentFilter.addAction(ZentriOSBLEService.ACTION_DISCONNECTED);
        mReceiverIntentFilter.addAction(ZentriOSBLEService.ACTION_ERROR);
        mReceiverIntentFilter.addAction(ZentriOSBLEService.TEMPEST_ACTION_UPDTAED);
        mReceiverIntentFilter.addAction(ZentriOSBLEService.ACTION_STRING_DATA_WRITE);
    }

    private void startBLEEnableIntent()
    {
        Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
        startActivityForResult(enableBtIntent, BLE_ENABLE_REQ_CODE);
    }

    private void initialiseListviewListener(ListView listView)
    {
        listView.setOnItemClickListener(new AdapterView.OnItemClickListener()
        {
            @Override
            public void onItemClick(AdapterView<?> parent, View view,
                                    int position, long id)
            {
                mCurrentDeviceName = mDeviceList.get(position);

                if (!mConnecting)
                {
                    mConnecting = true;

                    stopScan();
                    Log.d(TAG, "Connecting to BLE device " + mCurrentDeviceName);
                    mZentriOSBLEManager.connect(mCurrentDeviceName);

                    showConnectingDialog();

                    mHandler.postDelayed(mConnectTimeoutTask, CONNECT_TIMEOUT_MS);
                }
            }
        });
    }

    private void startScan()
    {
        if (mZentriOSBLEManager != null)
        {
            runOnUiThread(new Runnable()
            {
                @Override
                public void run()
                {
                    mZentriOSBLEManager.startScan();
                }
            });
            startProgressBar();
            disableScanButton();
            mHandler.postDelayed(mStopScanTask, SCAN_PERIOD);
        }
    }

    private void stopScan()
    {
        if (mZentriOSBLEManager != null && mZentriOSBLEManager.stopScan())
        {
            stopProgressBar();
            enableScanButton();
        }
    }

    private void showConnectingDialog()
    {
        runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                mConnectProgressDialog = Util.showProgressDialog(MainActivity.this,
                        R.string.progress_title,
                        R.string.progress_message);
            }
        });
    }

    private void startLocationEnableIntent()
    {
        Log.d(TAG, "Directing user to enable location services");
        Intent enableBtIntent = new Intent(android.provider.Settings.ACTION_LOCATION_SOURCE_SETTINGS);
        startActivityForResult(enableBtIntent, LOC_ENABLE_REQ_CODE);
    }

    private void showLocationEnableDialog()
    {
        runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                mLocationEnableDialog = new AlertDialog.Builder(MainActivity.this)
                        .setTitle(R.string.loc_enable_title)
                        .setMessage(R.string.loc_enable_msg)
                        .setPositiveButton(R.string.settings, new DialogInterface.OnClickListener()
                        {
                            @Override
                            public void onClick(DialogInterface dialog, int which)
                            {
                                startLocationEnableIntent();
                            }
                        })
                        .setNegativeButton(R.string.cancel, new DialogInterface.OnClickListener()
                        {
                            @Override
                            public void onClick(DialogInterface dialog, int which)
                            {
                                dialog.dismiss();
                                showErrorDialog(R.string.error_loc_disabled, true);
                            }
                        }).create();
                mLocationEnableDialog.show();
                Resources res = getResources();
                //Util.setTitleColour(res, mLocationEnableDialog, R.color.zentri_orange);
                //Util.setDividerColour(res, mLocationEnableDialog, R.color.transparent);
            }
        });
    }

    private boolean requestPermissions()
    {
        boolean result = true;

        if (ContextCompat.checkSelfPermission(MainActivity.this, LOC_PERM)
                != PackageManager.PERMISSION_GRANTED)
        {
            result = false;

            // Should we show an explanation?
            if (ActivityCompat.shouldShowRequestPermissionRationale(MainActivity.this, LOC_PERM))
            {

                // Show an explanation to the user
                showPermissionsRationaleDialog();
            }
            else
            {
                ActivityCompat.requestPermissions(MainActivity.this,
                        new String[]{LOC_PERM},
                        LOC_ENABLE_REQ_CODE);
            }
        }

        return result;
    }

    /**
     * Checks if requirements for this app to run are met.
     * @return true if requirements to run are met
     */
    private boolean requirementsMet()
    {
        boolean reqMet = false;

        if (!mZentriOSBLEManager.isInitialised())
        {
            startBLEEnableIntent();
        }
        else if (!requestPermissions())
        {
        }
        else if (!Util.isPreMarshmallow() && !Util.isLocationEnabled(this))
        {
            showLocationEnableDialog();
        }
        else
        {
            reqMet = true;
        }

        return reqMet;
    }

    private void showPermissionsRationaleDialog()
    {
        mPermissionRationaleDialog = new AlertDialog.Builder(MainActivity.this)
                .setTitle(R.string.permission_rationale_title)
                .setMessage(R.string.permission_rationale_msg)
                .setPositiveButton(R.string.enable, new DialogInterface.OnClickListener()
                {
                    @Override
                    public void onClick(DialogInterface dialog, int which)
                    {
                        dialog.dismiss();
                        ActivityCompat.requestPermissions(MainActivity.this,
                                new String[]{LOC_PERM},
                                LOC_ENABLE_REQ_CODE);
                    }
                })
                .setNegativeButton(R.string.cancel, new DialogInterface.OnClickListener()
                {
                    @Override
                    public void onClick(DialogInterface dialog, int which)
                    {
                        dialog.dismiss();
                        showErrorDialog(R.string.error_permission_denied, true);
                    }
                }).create();

        mPermissionRationaleDialog.show();
        Resources res = getResources();
        //Util.setTitleColour(res, mPermissionRationaleDialog, R.color.zentri_orange);
        //Util.setDividerColour(res, mPermissionRationaleDialog, R.color.transparent);
    }

    private void startDeviceInfoActivity()
    {
        //startActivity(new Intent(getApplicationContext(), DeviceInfoActivity.class));
    }

    private void startProgressBar()
    {
        updateProgressBar(true);
    }

    private void stopProgressBar()
    {
        updateProgressBar(false);
    }

    private void enableScanButton()
    {
        runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                //mScanButton.setEnabled(true);

            }
        });
    }

    private void disableScanButton()
    {
        runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                //mScanButton.setEnabled(false);
            }
        });
    }

    private void showToast(final String msg, final int duration)
    {
        runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                Toast.makeText(getApplicationContext(), msg, duration).show();
            }
        });
    }

    private void showErrorDialog(final int msgID, final boolean finishOnClose)
    {
        runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                Util.showErrorDialog(MainActivity.this, R.string.error, msgID, finishOnClose);
            }
        });
    }

    private void dismissDialog(final Dialog dialog)
    {
        runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                if (dialog != null)
                {
                    dialog.dismiss();
                }
            }
        });
    }

    //Only adds to the list if not already in it
    private void addDeviceToList(final String name)
    {
        runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                //mDeviceList.add(name);
            }
        });
    }

    private void updateProgressBar(final boolean start)
    {
        runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                if (start)
                {
                    //mScanProgressBar.progressiveStart();
                }
                else
                {
                    //mScanProgressBar.progressiveStop();
                }
            }
        });
    }

    private void openAboutDialog()
    {
        runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                Util.makeAboutDialog(MainActivity.this);
            }
        });
    }
}
