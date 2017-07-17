package com.example.jefferson.goodstracker.Activities;

import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.design.widget.BottomNavigationView;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.app.AppCompatActivity;
import android.view.MenuItem;
import android.view.View;

import com.example.jefferson.goodstracker.Communication.Communication;
import com.example.jefferson.goodstracker.Communication.TYPE_COMMUNICATION;
import com.example.jefferson.goodstracker.R;

public class GoodsTrackersActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_goods_trackers);

        Communication.create(TYPE_COMMUNICATION.AMQP);

        showFragment(new TrackerHome());

        BottomNavigationView navigation = (BottomNavigationView) findViewById(R.id.navigation);
        navigation.setOnNavigationItemSelectedListener(mOnNavigationItemSelectedListener);
    }

    public void onClick_btTrip(View view){

        showTripActivit();
    };

    public void onClick_btRabbit(View view){

        showServerActivity();
    };

    public void onClick_btSetup(View view){

        showSettingsActivit();
    };

    private void showServerActivity() {

        Intent it = new Intent(this,   RabbitActivity.class);
        startActivity(it);
    }

    public void showTripActivit(){

        Intent it = new Intent(this,   TripActivity.class);
        startActivity(it);
    }

    public void showSettingsActivit(){

        Intent it = new Intent(this,   SettingsActivity.class);
        startActivity(it);
    }

    /**
     * Handle bottom menu
     *
     */
    private BottomNavigationView.OnNavigationItemSelectedListener mOnNavigationItemSelectedListener
            = new BottomNavigationView.OnNavigationItemSelectedListener() {

        @Override
        public boolean onNavigationItemSelected(@NonNull MenuItem item) {
            switch (item.getItemId()) {

                case R.id.navigation_home:          showFragment(new TrackerHome());
                    return true;
                case R.id.navigation_dashboard:     showFragment(new TrackerDashBoard());
                    return true;
                case R.id.navigation_notifications: showFragment(new TrackerNotification());
                    return true;
            }
            return false;
        }
    };

    protected void showFragment(Fragment fragment) {

        FragmentTransaction t = getSupportFragmentManager().beginTransaction();
        t.replace(R.id.content, fragment);
        t.commit();
    }

    @Override
    protected void onDestroy() {

        super.onDestroy();

        Communication.create(TYPE_COMMUNICATION.NONE);
    };
}
