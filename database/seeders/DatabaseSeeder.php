<?php

namespace Database\Seeders;

// use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;

class DatabaseSeeder extends Seeder
{
    /**
     * Seed the application's database.
     *
     * @return void
     */
    public function run()
    {
        \App\Models\Client::factory(50)->create();
        
        $this->call([
            UserSeeder::class,
            FlameSeeder::class,
            LampSeeder::class,
            VaseSeeder::class,
            CrossSeeder::class,
            TombTypeSeeder::class,
            SculptureSeeder::class,
            ColorSeeder::class,
            FontSeeder::class,
            FinishingSeeder::class,
            MaterialSeeder::class,
            DecorationSeeder::class,
            OutlineSeeder::class,
            TombFrameSeeder::class,
            PictureSeeder::class,
        ]);
    }
}
