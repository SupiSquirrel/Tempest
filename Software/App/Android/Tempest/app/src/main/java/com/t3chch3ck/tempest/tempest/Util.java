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

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.res.Resources;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Build;
import android.provider.Settings;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

public class Util
{
    public static AlertDialog makeAboutDialog(Activity activity)
    {
        LayoutInflater inflater = activity.getLayoutInflater();

        AlertDialog dialog = new AlertDialog.Builder(activity)
                .setTitle(R.string.about)
                .setCancelable(true)
                .setView(inflater.inflate(R.layout.about_dialog, null))
                .create();

        return setAboutDialogContentsAndShow(activity, dialog);
    }

    private static AlertDialog setAboutDialogContentsAndShow(final Activity activity, AlertDialog dialog)
    {
        String app_name = activity.getString(R.string.app_name);
        String link = activity.getString(R.string.code_link_display);
        String version = getAppVersion(activity.getApplicationContext());
        String text = activity.getString(R.string.about_text);
        //String msg = String.format("%s\nv%s\n\n%s", app_name, text);
        String msg = String.format("%s\n", app_name);
        String msgLower = String.format("%s\n", text);

        dialog.show();//must show to be able to find views


        //TextView messageView = (TextView)dialog.findViewById(R.id.about_body);
        //messageView.setText(msg);

        TextView messageViewLower = (TextView)dialog.findViewById(R.id.about_body_lower);
        messageViewLower.setText(msgLower);

        ImageView imageView = (ImageView) dialog.findViewById(R.id.imageView);
        //imageView.setImageDrawable();
        String uri = "@drawable/tempestlogo";  // where myresource (without the extension) is the file
        int imageResource = activity.getResources().getIdentifier(uri, null, activity.getPackageName());
        //imageview= (ImageView)findViewById(R.id.imageView);
        Drawable imgres = activity.getResources().getDrawable(imageResource);
        imageView.setImageDrawable(imgres);


        final TextView linkView = (TextView)dialog.findViewById(R.id.about_link);
        linkView.setText(link);
        linkView.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                //go to webpage
                Intent intent = new Intent(Intent.ACTION_VIEW);
                intent.setData(Uri.parse(activity.getString(R.string.code_link)));
                activity.startActivity(intent);//open browser
            }
        });

        //remove divider in jellybean
        Resources res = activity.getResources();
        setTitleColour(res, dialog, R.color.colorPrimaryDark);
        setDividerColour(res, dialog, R.color.transparent);

        return dialog;
    }

    public static String getAppVersion(Context context)
    {
        PackageManager packageManager = context.getPackageManager();
        String packageName = context.getPackageName();

        String versionName = ""; // initialize String

        if (packageManager != null && packageName != null)
        {
            try
            {
                versionName = packageManager.getPackageInfo(packageName, 0).versionName;
            }
            catch (PackageManager.NameNotFoundException e)
            {
            }
        }

        return versionName;
    }

    public static Dialog showErrorDialog(final Activity activity, final int titleID,
                                         final int msgID, final boolean finishOnClose)
    {
        Dialog dialog = null;
        AlertDialog.Builder builder = new AlertDialog.Builder(activity);

        if (!activity.isFinishing())//make sure activity not finishing before showing
        {
            dialog = builder.setCancelable(false)
                    .setTitle(titleID)
                    .setMessage(msgID)
                    .setPositiveButton(R.string.ok, new DialogInterface.OnClickListener()
                   {
                       @Override
                       public void onClick(DialogInterface dialog, int which)
                       {
                           dialog.dismiss();
                           if (finishOnClose)
                           {
                               activity.finish();
                           }
                       }
                   }).create();
            dialog.show();

            Resources res = activity.getResources();
            //setTitleColour(res, dialog, R.color.zentri_orange);
            //setDividerColour(res, dialog, R.color.transparent);
        }

        return dialog;
    }

    //MUST be run from the UI thread!
    public static Dialog showProgressDialog(final Activity activity, final int titleID,
                                            final int msgID)
    {
        AlertDialog dialog = null;

        if (!activity.isFinishing())
        {
            /*LayoutInflater inflater = activity.getLayoutInflater();

            AlertDialog.Builder builder = new AlertDialog.Builder(activity);
            builder.setCancelable(true);
            builder.setView(inflater.inflate(R.layout.progress_dialog, null));

            dialog = builder.create();

            dialog.show();

            //cant set these until dialog has been shown
            TextView title = (TextView) dialog.findViewById(R.id.title);
            title.setText(titleID);

            TextView msg = (TextView) dialog.findViewById(R.id.message);
            msg.setText(msgID);
            dialog.setOnCancelListener(new DialogInterface.OnCancelListener()
            {
                @Override
                public void onCancel(DialogInterface dialogInterface)
                {
                    dialogInterface.dismiss();

                }
            });*/
        }

        return dialog;
    }

    private static void showToast(final Activity activity, final String msg, final int duration)
    {
        activity.runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                Toast.makeText(activity, msg, duration).show();
            }
        });
    }

    static boolean isPreMarshmallow()
    {
        return (Build.VERSION.SDK_INT <= Build.VERSION_CODES.LOLLIPOP_MR1);
    }

    public static boolean isLocationEnabled(Context context)
    {
        int locationMode = Settings.Secure.LOCATION_MODE_OFF;
        String locationProviders;

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT)
        {
            try
            {
                locationMode = Settings.Secure.getInt(context.getContentResolver(),
                        Settings.Secure.LOCATION_MODE);

            } catch (Settings.SettingNotFoundException e)
            {
                e.printStackTrace();
            }

            return locationMode != Settings.Secure.LOCATION_MODE_OFF;
        }
        else
        {
            locationProviders = Settings.Secure.getString(context.getContentResolver(),
                    Settings.Secure.LOCATION_PROVIDERS_ALLOWED);
            return !TextUtils.isEmpty(locationProviders);
        }
    }

    static void setDividerColour(Resources res, Dialog dialog, int colourID)
    {
        final View divider = getElement(res, "titleDivider", dialog);
        int color = res.getColor(colourID);
        if (divider != null)
        {
            divider.setBackgroundColor(color);
        }
    }

    static void setTitleColour(Resources res, Dialog dialog, int colourID)
    {
        final TextView title = (TextView)getElement(res, "alertTitle", dialog);
        int color = res.getColor(colourID);
        if (title != null)
        {
            title.setTextColor(color);
        }
    }

    private static View getElement(Resources res, String id, Dialog dialog)
    {
        final int element_id = res.getIdentifier(id, "id", "android");
        return dialog.findViewById(element_id);
    }
}
