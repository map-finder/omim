package com.mapswithme.maps.widget;

import android.app.Dialog;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.DialogFragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;

import com.mapswithme.maps.R;
import com.mapswithme.util.Config;
import com.mapswithme.util.NetworkPolicy;
import com.mapswithme.util.statistics.Statistics;

public class StackedButtonDialogFragment extends DialogFragment
{

  @Nullable
  private NetworkPolicy.NetworkPolicyListener mListener;

  @NonNull
  @Override
  public Dialog onCreateDialog(Bundle savedInstanceState)
  {
    return new StackedButtonsDialog.Builder(getContext())
        .setTitle(R.string.mobile_data_dialog)
        .setMessage(R.string.mobile_data_description)
        .setCancelable(false)
        .setPositiveButton(R.string.mobile_data_option_always,
                           (dialog, which) -> onDialogBtnClicked(NetworkPolicy.Type.ALWAYS, true))
        .setNegativeButton(R.string.mobile_data_option_not_today,
                           (dialog, which) -> onMobileDataImpactBtnClicked(NetworkPolicy.Type.NOT_TODAY, false))
        .setNeutralButton(R.string.mobile_data_option_today,
                          (dialog, which) -> onMobileDataImpactBtnClicked(NetworkPolicy.Type.TODAY, true))
        .build();
  }

  private void onMobileDataImpactBtnClicked(@NonNull NetworkPolicy.Type today, boolean canUse)
  {
    Config.setMobileDataTimeStamp(System.currentTimeMillis());
    onDialogBtnClicked(today, canUse);
  }

  private void onDialogBtnClicked(@NonNull NetworkPolicy.Type type, boolean canUse)
  {
    Statistics.INSTANCE.trackNetworkUsageAlert(Statistics.EventName.MOBILE_INTERNET_ALERT,
                                               type.toStatisticValue());
    Config.setUseMobileDataSettings(type);
    if (mListener != null)
      mListener.onResult(NetworkPolicy.newInstance(canUse));

  }

  @Override
  public void show(@NonNull FragmentManager manager, @NonNull String tag)
  {
    FragmentTransaction ft = manager.beginTransaction();
    ft.add(this, tag);
    ft.commitAllowingStateLoss();
  }

  public void setListener(@Nullable NetworkPolicy.NetworkPolicyListener listener)
  {
    mListener = listener;
  }
}
