package com.mapswithme.maps.purchase;

import android.content.Context;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import com.android.billingclient.api.BillingClient;
import com.mapswithme.maps.PrivateVariables;
import com.mapswithme.maps.PurchaseOperationObservable;
import com.mapswithme.util.Utils;

public class PurchaseFactory
{
  private PurchaseFactory()
  {
    // Utility class.
  }

  @NonNull
  public static PurchaseController<PurchaseCallback> createAdsRemovalPurchaseController(
      @NonNull Context context)
  {
    BillingManager<PlayStoreBillingCallback> billingManager
        = new PlayStoreBillingManager(BillingClient.SkuType.SUBS);
    PurchaseOperationObservable observable = PurchaseOperationObservable.from(context);
    PurchaseValidator<ValidationCallback> validator = new DefaultPurchaseValidator(observable);
    String yearlyProduct = PrivateVariables.adsRemovalYearlyProductId();
    String monthlyProduct = PrivateVariables.adsRemovalMonthlyProductId();
    String weeklyProduct = PrivateVariables.adsRemovalWeeklyProductId();
    String[] productIds = Utils.concatArrays(PrivateVariables.adsRemovalNotUsedList(),
                                             yearlyProduct, monthlyProduct, weeklyProduct);
    return new SubscriptionPurchaseController(validator, billingManager,
                                              SubscriptionType.ADS_REMOVAL, productIds);
  }

  @NonNull
  public static PurchaseController<PurchaseCallback> createBookmarksSubscriptionPurchaseController(
      @NonNull Context context)
  {
    BillingManager<PlayStoreBillingCallback> billingManager
        = new PlayStoreBillingManager(BillingClient.SkuType.SUBS);
    PurchaseOperationObservable observable = PurchaseOperationObservable.from(context);
    PurchaseValidator<ValidationCallback> validator = new DefaultPurchaseValidator(observable);
    String yearlyProduct = PrivateVariables.bookmarksSubscriptionYearlyProductId();
    String monthlyProduct = PrivateVariables.bookmarksSubscriptionMonthlyProductId();
    String[] productIds = Utils.concatArrays(PrivateVariables.bookmarksSubscriptionNotUsedList(),
                                             yearlyProduct, monthlyProduct);
    return new SubscriptionPurchaseController(validator, billingManager,
                                              SubscriptionType.BOOKMARKS, productIds);
  }

  @NonNull
  public static PurchaseController<PurchaseCallback> createBookmarkPurchaseController(
      @NonNull Context context, @Nullable String productId, @Nullable String serverId)
  {
    BillingManager<PlayStoreBillingCallback> billingManager
        = new PlayStoreBillingManager(BillingClient.SkuType.INAPP);
    PurchaseOperationObservable observable = PurchaseOperationObservable.from(context);
    PurchaseValidator<ValidationCallback> validator = new DefaultPurchaseValidator(observable);
    return new BookmarkPurchaseController(validator, billingManager, productId, serverId);
  }

  @NonNull
  public static PurchaseController<PurchaseCallback> createBookmarkPurchaseController(
      @NonNull Context context)
  {
    return createBookmarkPurchaseController(context, null, null);
  }

  @NonNull
  public static PurchaseController<FailedPurchaseChecker> createFailedBookmarkPurchaseController(
      @NonNull Context context)
  {
    BillingManager<PlayStoreBillingCallback> billingManager
      = new PlayStoreBillingManager(BillingClient.SkuType.INAPP);
    PurchaseOperationObservable observable = PurchaseOperationObservable.from(context);
    PurchaseValidator<ValidationCallback> validator = new DefaultPurchaseValidator(observable);
    return new FailedBookmarkPurchaseController(validator, billingManager);
  }

  @NonNull
  public static BillingManager<PlayStoreBillingCallback> createInAppBillingManager(
      @NonNull Context context)
  {
    return new PlayStoreBillingManager(BillingClient.SkuType.INAPP);
  }

  @NonNull
  public static BillingManager<PlayStoreBillingCallback> createSubscriptionBillingManager()
  {
    return new PlayStoreBillingManager(BillingClient.SkuType.SUBS);
  }
}
