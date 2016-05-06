package com.t3chch3ck.tempest.tempest;

import android.app.Activity;
import android.content.Context;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.Spinner;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.List;


/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * to handle interaction events.
 * Use the {@link TempestSettingsFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class TempestSettingsFragment extends Fragment {
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    private static final String TAG = TempestSettingsFragment.class.getSimpleName();

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    //private OnFragmentInteractionListener mListener;

    private Spinner spinnerFiringMode;

    private SeekBar seekBarMaxRofEyesOn;
    private TextView textViewMaxRofEyesOn;

    private SeekBar seekBarMaxRofEyesOff;
    private TextView textViewMaxRofEyesOff;

    private SeekBar seekbarRampActivation;
    private TextView textViewRampActivation;

    private Button   buttonMinusRampAct;
    private Button   buttonPlusRampAct;
    private TextView textViewRampAct;

    private Button   buttonMinusDwell;
    private Button   buttonPlusDwell;
    private TextView textViewDwell;

    private static final int MIN_RAMP_ACTIVATION_SHOTS = 2;
    private static final int MAX_RAMP_ACTIVATION_SHOTS = 5;

    private static final int MIN_DWELL = 5;
    private static final int MAX_DWELL = 25;

    private int mRampAct = 2;
    private int mDwell = 5;

    public TempestSettingsFragment() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment TempestSettingsFragment.
     */
    // TODO: Rename and change types and number of parameters
    public static TempestSettingsFragment newInstance(String param1, String param2) {
        TempestSettingsFragment fragment = new TempestSettingsFragment();
        Bundle args = new Bundle();
        args.putString(ARG_PARAM1, param1);
        args.putString(ARG_PARAM2, param2);
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {
            mParam1 = getArguments().getString(ARG_PARAM1);
            mParam2 = getArguments().getString(ARG_PARAM2);
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        View view = inflater.inflate(R.layout.fragment_tempest_settings, container, false);

        spinnerFiringMode = (Spinner) view.findViewById(R.id.spinnerFiringMode);

        //         <item>Semi</item>
        //         <item>Ramp</item>
        //         <item>Burst</item>
        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(this.getActivity(),
                R.array.firing_modes_array, android.R.layout.simple_spinner_item);

        // Specify the layout to use when the list of choices appears
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

        // Apply the adapter to the spinner
        spinnerFiringMode.setAdapter(adapter);

        spinnerFiringMode.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener()
        {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {

                /*switch (position) {
                    case 0:
                        //Log.i(TAG, "onItemSelected 0 ");
                        break;
                    case 1:

                        break;
                    case 2:

                        break;
                }*/
            }
            @Override
            public void onNothingSelected(AdapterView<?> parent)
            {
                //
            }
        });


        seekBarMaxRofEyesOn    = (SeekBar) view.findViewById(R.id.seekBarMaxRofEyesOn);
        textViewMaxRofEyesOn   = (TextView) view.findViewById(R.id.textViewMaxRofEyesOn);

        seekBarMaxRofEyesOff   = (SeekBar) view.findViewById(R.id.seekBarMaxRofEyesOff);
        textViewMaxRofEyesOff  = (TextView) view.findViewById(R.id.textViewMaxRofEyesOff);

        seekbarRampActivation  = (SeekBar) view.findViewById(R.id.seekbarRampActivation);
        textViewRampActivation = (TextView) view.findViewById(R.id.textViewRampActivation);


        setupSeekbar(seekBarMaxRofEyesOn, 5.0, 20.0, 0.1);

        setupSeekbar(seekBarMaxRofEyesOff, 5.0, 15.0, 0.1);

        setupSeekbar(seekbarRampActivation, 1.0, 10.0, 0.1);

        buttonMinusRampAct = (Button) view.findViewById(R.id.buttonMinusRampAct);
        buttonPlusRampAct  = (Button) view.findViewById(R.id.buttonPlusRampAct);
        textViewRampAct    = (TextView) view.findViewById(R.id.textViewRampAct);

        buttonMinusDwell   = (Button) view.findViewById(R.id.buttonMinusDwell);
        buttonPlusDwell    = (Button) view.findViewById(R.id.buttonPlusDwell);
        textViewDwell      = (TextView) view.findViewById(R.id.textViewDwell);


        seekBarMaxRofEyesOn.setOnSeekBarChangeListener( new SeekBar.OnSeekBarChangeListener(){
            public void onProgressChanged(SeekBar seekBar, int progress,boolean fromUser)
            {
                textViewMaxRofEyesOn.setText(String.format("%2.1f", getDouble(progress, 5.0, 0.1)));
            }
            public void onStartTrackingTouch(SeekBar seekBar)
            { }
            public void onStopTrackingTouch(SeekBar seekBar)
            {
                mOnMaxRofEyesOnChangedCallback.onMaxRofEyesOnChanged(getDouble(seekBar.getProgress(), 5.0, 0.1));
            }
        });

        seekBarMaxRofEyesOff.setOnSeekBarChangeListener( new SeekBar.OnSeekBarChangeListener(){
            public void onProgressChanged(SeekBar seekBar, int progress,boolean fromUser)
            {
                textViewMaxRofEyesOff.setText(String.format("%2.1f", getDouble(progress, 5.0, 0.1)));
            }
            public void onStartTrackingTouch(SeekBar seekBar)
            { }
            public void onStopTrackingTouch(SeekBar seekBar)
            {
                mOnMaxRofEyesOnChangedCallback.onMaxRofEyesOffChanged(getDouble(seekBar.getProgress(), 5.0, 0.1));
            }
        });

        seekbarRampActivation.setOnSeekBarChangeListener( new SeekBar.OnSeekBarChangeListener(){
            public void onProgressChanged(SeekBar seekBar, int progress,boolean fromUser)
            {
                textViewRampActivation.setText(String.format("%2.1f", getDouble(progress, 1.0, 0.1)));
            }
            public void onStartTrackingTouch(SeekBar seekBar)
            { }
            public void onStopTrackingTouch(SeekBar seekBar)
            {
                mOnMaxRofEyesOnChangedCallback.onRampActivationChanged(getDouble(seekBar.getProgress(), 1.0, 0.1));
            }
        });

        updateMaxRofEyesOn(5.0);
        updateMaxRofEyesOff(5.0);
        updateRampActivation(1.0);


        buttonMinusRampAct.setOnClickListener( new OnClickListener(){
            public void onClick(View v) {
                if (mRampAct > MIN_RAMP_ACTIVATION_SHOTS) {
                    mRampAct--;
                    updateRampActtivationShots(mRampAct);
                    mOnMaxRofEyesOnChangedCallback.onRampActivationShotsChanged(mRampAct);
                }
            }
        });

        buttonPlusRampAct.setOnClickListener( new OnClickListener(){
            public void onClick(View v) {
                if (mRampAct < MAX_RAMP_ACTIVATION_SHOTS) {
                    mRampAct++;
                    updateRampActtivationShots(mRampAct);
                    mOnMaxRofEyesOnChangedCallback.onRampActivationShotsChanged(mRampAct);
                }
            }
        });

        buttonMinusDwell.setOnClickListener( new OnClickListener(){
            public void onClick(View v) {
                if (mDwell > MIN_DWELL) {
                    mDwell--;
                    updateDwell(mDwell);
                    mOnMaxRofEyesOnChangedCallback.onDwellChanged(mDwell);
                }
            }
        });

        buttonPlusDwell.setOnClickListener( new OnClickListener(){
            public void onClick(View v) {
                if (mDwell < MAX_DWELL) {
                    mDwell++;
                    updateDwell(mDwell);
                    mOnMaxRofEyesOnChangedCallback.onDwellChanged(mDwell);
                }
            }
        });

        updateRampActtivationShots(this.mRampAct);
        updateDwell(this.mDwell);


        return view;
    }

    public void updateRampActtivationShots(int value) {
        mRampAct = value;
        if (textViewRampAct != null) {
            textViewRampAct.setText(String.format("%2.0f", (double)value));
        }
    }

    public void updateDwell(int value) {
        mDwell = value;
        if (textViewDwell != null) {
            textViewDwell.setText(String.format("%2.0f", (double)value));
        }
    }

    public void updateMaxRofEyesOn(double value) {
        if ((textViewMaxRofEyesOn != null) && (seekBarMaxRofEyesOn != null)) {
            textViewMaxRofEyesOn.setText(String.format("%2.1f", value));
            seekBarMaxRofEyesOn.setProgress(getProgress(value, 5.0, 0.1));
        } else {
            Log.e(TAG, "updateMaxRofEyesOn: null");
        }
    }

    public void updateMaxRofEyesOff(double value) {
        if ((textViewMaxRofEyesOff != null) && (seekBarMaxRofEyesOff != null)) {
            textViewMaxRofEyesOff.setText(String.format("%2.1f", value));
            seekBarMaxRofEyesOff.setProgress(getProgress(value, 5.0, 0.1));
        } else {
            Log.e(TAG, "updateMaxRofEyesOff: null");
        }
    }

    public void updateRampActivation(double value) {
        if ((textViewRampActivation != null) && (seekbarRampActivation != null)) {
            textViewRampActivation.setText(String.format("%2.1f", value));
            seekbarRampActivation.setProgress(getProgress(value, 1.0, 0.1));
        } else {
            Log.e(TAG, "updateRampActivation: null");
        }
    }



    private int getProgress(double value, double min, double delta) {
        return ((int)(Math.round(value / delta)) - (int)(Math.round(min / delta)));
    }

    private int getSteps(double min, double max, double delta) {
        return ((int)(Math.round(max/delta)))-((int)(Math.round(min/delta)));
    }

    private double getDouble(int steps, double min, double delta) {
        return steps * delta + min;
    }

    private void setupSeekbar(SeekBar seekBar, double min, double max, double delta) {
        seekBar.setMax(getSteps(min, max, delta));
    }




    OnMaxRofEyesOnChanged   mOnMaxRofEyesOnChangedCallback;

    public interface OnMaxRofEyesOnChanged {
        public void onMaxRofEyesOnChanged(double value);
        public void onMaxRofEyesOffChanged(double value);
        public void onRampActivationChanged(double value);

        public void onDwellChanged(int value);
        public void onRampActivationShotsChanged(int value);
    }


    @Override
    public void onAttach(Context context) {
        super.onAttach(context);

        if (context instanceof OnMaxRofEyesOnChanged) {
            mOnMaxRofEyesOnChangedCallback = (OnMaxRofEyesOnChanged) context;
            Log.e(TAG, "register mOnMaxRofEyesOnChangedCallback");
        }  else {
            throw new RuntimeException(context.toString()
                    + " must implement OnFragmentInteractionListener");
        }
    }
}
