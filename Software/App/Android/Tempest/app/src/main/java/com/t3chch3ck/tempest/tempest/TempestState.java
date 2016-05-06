package com.t3chch3ck.tempest.tempest;

/**
 * Created by Squirrel on 23.04.2016.
 */
public class TempestState {
    private double triggerBps;
    private double solenoidBps;
    private double maxTriggerBps;

    private boolean triggerPulled;
    private boolean eyeActive;

    private double batteryVoltage;

    private int shotCount;
    private int sessionShotCount;

    private int maxRofEyesOn;
    private int maxRofEyesOff;
    private int rampKickinRof;
    private int rampKickinShots;
    private int dwell;
    private int firingMode;

    public TempestState() {
        triggerBps          = 0.0;
        solenoidBps         = 0.0;
        maxTriggerBps       = 0.0;
        triggerPulled       = false;
        eyeActive           = false;
        batteryVoltage      = 0.0;
        shotCount           = 0;
        sessionShotCount    = 0;
        setMaxRofEyesOn(0);
        setMaxRofEyesOff(0);
        setRampKickinRof(0);
        setRampKickinShots(0);
        setDwell(0);
        setFiringMode(0);
    }


    public double getTriggerBps() {
        return triggerBps;
    }

    public void setTriggerBps(double triggerBps) {
        this.triggerBps = triggerBps;
        if (triggerBps>this.maxTriggerBps) {
            this.maxTriggerBps = triggerBps;
        }
    }

    public int getSessionShotCount()  {
        return sessionShotCount;
    }

    public double getSolenoidBps() {
        return solenoidBps;
    }

    public void setSolenoidBps(double solenoidBps) {
        this.solenoidBps = solenoidBps;
    }

    public double getMaxTriggerBps() {
        return maxTriggerBps;
    }

    public void setMaxTriggerBps(double maxTriggerBps) {
        this.maxTriggerBps = maxTriggerBps;
    }

    public boolean isTriggerPulled() {
        return triggerPulled;
    }

    public void setTriggerPulled(boolean triggerPulled) {
        this.triggerPulled = triggerPulled;
    }

    public boolean isEyeActive() {
        return eyeActive;
    }

    public void setEyeActive(boolean eyeActive) {
        this.eyeActive = eyeActive;
    }

    public double getBatteryVoltage() {
        return batteryVoltage;
    }

    public void setBatteryVoltage(double batteryVoltage) {
        this.batteryVoltage = batteryVoltage;
    }

    public int getShotCount() {
        return shotCount;
    }

    public void setShotCount(int shotCount) {
        this.shotCount = shotCount;
    }

    public int getMaxRofEyesOn() {
        return maxRofEyesOn;
    }

    public void setMaxRofEyesOn(int maxRofEyesOn) {
        this.maxRofEyesOn = maxRofEyesOn;
    }

    public int getMaxRofEyesOff() {
        return maxRofEyesOff;
    }

    public void setMaxRofEyesOff(int maxRofEyesOff) {
        this.maxRofEyesOff = maxRofEyesOff;
    }

    public int getRampKickinRof() {
        return rampKickinRof;
    }

    public void setRampKickinRof(int rampKickinRof) {
        this.rampKickinRof = rampKickinRof;
    }

    public int getRampKickinShots() {
        return rampKickinShots;
    }

    public void setRampKickinShots(int rampKickinShots) {
        this.rampKickinShots = rampKickinShots;
    }

    public int getDwell() {
        return dwell;
    }

    public void setDwell(int dwell) {
        this.dwell = dwell;
    }

    public int getFiringMode() {
        return firingMode;
    }

    public void setFiringMode(int firingMode) {
        this.firingMode = firingMode;
    }
}
