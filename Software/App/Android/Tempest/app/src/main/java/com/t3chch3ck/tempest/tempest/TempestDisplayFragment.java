package com.t3chch3ck.tempest.tempest;

import android.os.Bundle;
import android.support.v4.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.RadioButton;
import android.widget.TextView;


/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * {@link TempestDisplayFragment.OnFragmentInteractionListener} interface
 * to handle interaction events.
 * Use the {@link TempestDisplayFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class TempestDisplayFragment extends Fragment {
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    //private OnFragmentInteractionListener mListener;

    public TempestDisplayFragment() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment TempestDisplayFragment.
     */
    // TODO: Rename and change types and number of parameters
    public static TempestDisplayFragment newInstance(String param1, String param2) {
        TempestDisplayFragment fragment = new TempestDisplayFragment();
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

    TextView    textViewTriggerRof;
    TextView    textViewMaxTriggerRof;
    TextView    textViewSolenoidRof;
    TextView    textViewMaxSolenoidRof;
    TextView    textViewShotCounter;
    TextView    textViewSessionShotCounter;
    TextView    textViewBatteryVoltage;
    RadioButton radioButtonTriggerState;
    RadioButton radioButtonEyeState;


    public void setTriggerRof(double value) {
        String textValue = String.format("%2.1f", value);
        textViewTriggerRof.setText(textValue);
    }

    public void updateDisplay(TempestState tempestState) {
        textViewTriggerRof.setText(String.format("%2.1f", tempestState.getTriggerBps()));
        textViewMaxTriggerRof.setText(String.format("%2.1f", tempestState.getMaxTriggerBps()));

        textViewSolenoidRof.setText(String.format("%2.1f", tempestState.getSolenoidBps()));

        textViewShotCounter.setText(String.format("%d", tempestState.getShotCount()));
        textViewSessionShotCounter.setText(String.format("%d", tempestState.getSessionShotCount()));

        textViewBatteryVoltage.setText(String.format("%1.1f", tempestState.getBatteryVoltage()));

        radioButtonTriggerState.setChecked(tempestState.isTriggerPulled());
        radioButtonEyeState.setChecked(tempestState.isEyeActive());
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_tempest_display, container, false);

        textViewTriggerRof = (TextView) view.findViewById(R.id.textViewTriggerRof);
        textViewMaxTriggerRof = (TextView) view.findViewById(R.id.textViewMaxTriggerRof);

        textViewSolenoidRof = (TextView) view.findViewById(R.id.textViewMaxRofEyesOn);
        textViewMaxSolenoidRof = (TextView) view.findViewById(R.id.textViewMaxSolenoidRof);

        textViewShotCounter = (TextView) view.findViewById(R.id.textViewShotCounter);
        textViewSessionShotCounter = (TextView) view.findViewById(R.id.textViewSessionShotCounter);

        textViewBatteryVoltage = (TextView) view.findViewById(R.id.textViewBatteryVoltage);

        radioButtonTriggerState = (RadioButton) view.findViewById(R.id.radioButtonTriggerState);
        radioButtonEyeState = (RadioButton) view.findViewById(R.id.radioButtonEyeState);

        return view;
    }

    // TODO: Rename method, update argument and hook method into UI event
   /* public void onButtonPressed(Uri uri) {
        if (mListener != null) {
            mListener.onFragmentInteraction(uri);
        }
    }

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
        if (context instanceof OnFragmentInteractionListener) {
            mListener = (OnFragmentInteractionListener) context;
        } else {
            throw new RuntimeException(context.toString()
                    + " must implement OnFragmentInteractionListener");
        }
    }

    @Override
    public void onDetach() {
        super.onDetach();
        mListener = null;
    }*/

    /**
     * This interface must be implemented by activities that contain this
     * fragment to allow an interaction in this fragment to be communicated
     * to the activity and potentially other fragments contained in that
     * activity.
     * <p/>
     * See the Android Training lesson <a href=
     * "http://developer.android.com/training/basics/fragments/communicating.html"
     * >Communicating with Other Fragments</a> for more information.
     */
    /*public interface OnFragmentInteractionListener {
        // TODO: Update argument type and name
        void onFragmentInteraction(Uri uri);
    }*/
}
